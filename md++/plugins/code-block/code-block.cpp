// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <string>
#define MDXX_EXTERNAL_CONTEXT
#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include "plugin_loader.h"
#include "variable_map.h"
#include "thread_safe_print.h"
#include "split.h"
#include "py_init.h"
#include <iostream>
#include <stdexcept>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

void thread_safe_py_err_print() {
	#pragma omp critical(thread_safe_printing)
	{
		#pragma omp critical(python)
		PyErr_Print();
	}
}

class Code_Block;

template<>
const char * mdxx::Expansion<Code_Block*>::to_string();

class Code_Block : public mdxx::Context {
public:
	Code_Block(const char * n, mdxx::Plugin_Loader * p, mdxx::MDXX_Manager * m) : name(n), pl(p), md(m) {
		variables = MDXX_get_variable_map(pl, this);
		add_variable("{", "{");
		add_variable("}", "}");
		add_variable("\\}", "\\}");
		add_variable("lt", "<");
		add_variable("gt", ">");
		add_variable("amp", "&");
	}

	void open(mdxx::HTML_Manager& html, const char * arg_ptr) override {
		(void)html;
		std::vector<std::string> args = mdxx::split(arg_ptr);
		if (args.size() < 2) {
			MDXX_error(md,
				"Need to specify the programming language for the code-block context. Proper usage would be\n"
				"\t{{open code-block c++}}\n"
				"You can also remove line numbers by adding the argument `no-line-numbers` after the `code-block`.\n"
				"Also, make sure that the code style is the last argument or else you'll get an error saying\n"
				"something like `pygments.util.ClassNotFound: no lexer for alias 'code-style' found`");
			thread_safe_py_err_print();
			return;
		}
		code_language = args[0];
		new_code_block_initialization();
		if (args.size() > 1) {
			for (size_t i = 1; i < args.size(); i++) {
				if (args[i] == "no-line-numbers") {
					add_line_numbers = false;
				} else {
					code_style = args[i];
				}
			}
		}
		if (code_style == "") {
			MDXX_error(md,
				"You must specify the desired code style when you first open a code-block.\n"
				"Example:\n"
				"\t{{open code-block c++ trac}}\n"
				"where \"trac\" is the preferred code-style.");
			return;
		}
		if (add_line_numbers) {
			MDXX_html_add(&html, "<div class=\"code-blocktable\">");
		}
	}

	void process(mdxx::HTML_Manager& html, const char * line, size_t num_lines) override {
		(void)html;
		if (line[0] == '\0') {
			return;
		}
		bool highlight_line = (line[0] == 'h' && line[1] == 'l');
		const char * line_start = line + highlight_line * 2;
		if (highlight_line) {
			lines_to_highlight.push_back(current_line_num);
		}
		if (!first_line) {
			current_line_num += num_lines;
			code_block += std::string(num_lines - 1, '\n');
		}
		code_block += line_start;
		code_block += '\n';
		first_line = false;
	}

	void close(mdxx::HTML_Manager& html) override {
		#pragma omp critical(python)
		MDXX_py_init();
		PyObject * myModule = NULL;
		#pragma omp critical(python)
		myModule = PyImport_ImportModule("code_block");
		if (myModule == NULL) {
			std::string error_message;
			error_message.reserve(2048);
			error_message += "Could not load `code_block.py`. It should be in the directory\n\n\t`";
			error_message += (fs::absolute(MDXX_get_main_program_dir()) / "plugins").string();
			error_message += "`\n\n"
				"If you aren't building from source, then you can find it at\n\n"
				"\thttps://josephmellor.xyz/mdxx/\n\n"
				"Download the file and copy or move it to the directory listed above.\n"
				"If building from source, build the install target. On Linux, type\n\n\t`$ make install`\n\nin the same directory you typed `make`. On Windows, build target INSTALL.\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
			return;
		}
		if (pygments_wrapper == NULL) {
			#pragma omp critical(python)
			pygments_wrapper = PyObject_GetAttrString(myModule, (char*)"hand_code_to_pygments");
		}
		if (pygments_wrapper == NULL) {
			MDXX_error(md,
			"Could not find method \"hand_code_to_pygments\", which"
			"shouldn't happen.\nYou're on your own.\n");
			thread_safe_py_err_print();
			return;
		}
		PyObject* py_lines_to_highlight = NULL;
		#pragma omp critical(python)
		py_lines_to_highlight = PyList_New(lines_to_highlight.size());
		if (py_lines_to_highlight == NULL) {
			MDXX_error(md,
				"Could not create python list. I have no idea why.");
			thread_safe_py_err_print();
			return;
		}
		for (size_t i = 0; i < lines_to_highlight.size(); i++) {
			PyObject* py_current_line;
			#pragma omp critical(python)
			py_current_line = PyLong_FromUnsignedLong(lines_to_highlight[i]);
			if (py_current_line == NULL) {
				std::string error_message;
				error_message.reserve(2048);
				error_message += "Could not convert lines_to_highlight[";
				error_message += std::to_string(i);
				error_message += "] to a PyLong. I have no idea why.\n";
				error_message += "\tlines_to_highlight[";
				error_message += std::to_string(i);
				error_message += "]:\t";
				error_message += lines_to_highlight[i];
				error_message += "\n";
				MDXX_error(md, error_message.c_str());
				thread_safe_py_err_print();
				return;
			}
			int py_list_err_code;
			#pragma omp critical(python)
			py_list_err_code = PyList_SetItem(py_lines_to_highlight, i, py_current_line);
			if (py_list_err_code == -1) {
				std::string error_message;
				error_message.reserve(2048);
				error_message += "Tried to insert object at index ";
				error_message += std::to_string(i);
				error_message += ", which is out of bounds for the list.\n"
				"This shouldn't happen, but it might be something I could solve.\n"
				"Size of list should be ";
				error_message += std::to_string(lines_to_highlight.size());
				error_message += ".\n";
				MDXX_error(md, error_message.c_str());
				thread_safe_py_err_print();
				return;
			}
		}
		PyObject* py_code_block;
		#pragma omp critical(python)
		py_code_block = PyUnicode_FromString(code_block.c_str());
		if (py_code_block == NULL) {
			std::string error_message;
			error_message.reserve(code_block.length() + 100);
			error_message += "Could not convert code_block to python string. code_block is `";
			error_message += code_block;
			error_message += "`\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
			return;
		}
		PyObject* py_code_language;
		#pragma omp critical(python)
		py_code_language = PyUnicode_FromString(code_language.c_str());
		if (py_code_language == NULL) {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "Could not convert code_language to python string. code_language is `";
			error_message += code_language;
			error_message += "`\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
			return;
		}
		PyObject* py_code_style;
		#pragma omp critical(python)
		py_code_style = PyUnicode_FromString(code_style.c_str());
		if (py_code_style == NULL) {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "Could not convert code_style to python string. code_style is `";
			error_message += code_style.c_str();
			error_message += "`\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
			return;
		}
		PyObject * py_add_line_numbers;
		#pragma omp critical(python)
		py_add_line_numbers = PyBool_FromLong(add_line_numbers);
		if (py_add_line_numbers == NULL) {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "Could not convert add_line_numbers to python bool. add_line_numbers is `";
			error_message += add_line_numbers;
			error_message += "`\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
			return;
		}
		PyObject * args;
		#pragma omp critical(python)
		args = PyTuple_Pack(5, py_code_block, py_code_language, py_add_line_numbers, py_code_style, py_lines_to_highlight);
		if (args == NULL) {
			std::stringstream strstr;
			strstr << "Could not pack the arguments to hand_code_to_pygments." << std::endl;
			strstr << "\t`" << code_block
				<< "`\n\t`" << code_language
				<< "`\n\t`" << add_line_numbers
				<< "`\n\t`" << code_style
				<< "`\n\t`" << py_lines_to_highlight
				<< "`" << std::endl;
			MDXX_error(md, strstr.str().c_str());
			thread_safe_py_err_print();
			return;
		}
		PyObject* myResult;
		#pragma omp critical(python)
		myResult = PyObject_CallObject(pygments_wrapper, args);
		if (myResult == NULL) {
			std::stringstream strstr;
			strstr << "hand_code_to_pygments was unable to produce output." << std::endl;
			strstr << "\t`" << code_block
				<< "`\n\t`" << code_language
				<< "`\n\t`" << add_line_numbers
				<< "`\n\t`" << code_style
				<< "`\n\t`" << py_lines_to_highlight
				<< "`" << std::endl;
			MDXX_error(md, strstr.str().c_str());
			thread_safe_py_err_print();
			return;
		}

		Py_ssize_t size;
		const char *ptr;
		#pragma omp critical(python)
		ptr = PyUnicode_AsUTF8AndSize(myResult, &size);
		if (ptr == NULL) {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "pygments returned something that could not be written in UTF8. The output of pygments is `";
			error_message += ptr;
			error_message += "`\n";
			MDXX_error(md, error_message.c_str());
			thread_safe_py_err_print();
		} else {
			MDXX_html_write(&html, ptr);
			MDXX_html_add(&html, "");
			if (add_line_numbers) {
				MDXX_html_add(&html, "</div>");
			}
		}
	}

	void new_code_block_initialization() {
		code_block = "";
		first_line = true;
		current_line_num = 1;
		lines_to_highlight.clear();
		add_line_numbers = true;
	}

	~Code_Block() {
		MDXX_free_variable_map(pl, this);
	}

	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
public:
	std::string code_block_object_id;
private:
	mdxx::Plugin_Loader * pl;
	mdxx::MDXX_Manager * md;
	std::string code_language;
	std::string code_style = "";
	std::string code_block;
	uint32_t current_line_num = 1;
	bool first_line = true;
	bool add_line_numbers = true;
	std::vector<uint32_t> lines_to_highlight;
	static PyObject * pygments_wrapper;
};

PyObject * Code_Block::pygments_wrapper = NULL;

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Code_Block)

extern "C" DLL_IMPORT_EXPORT void import_plugin(mdxx::Plugin_Loader * pl, mdxx::MDXX_Manager * mdxx) {
	MDXX_add_new_context(mdxx, "code-block", new Code_Block("code-block", pl, mdxx));
}

extern "C" DLL_IMPORT_EXPORT void print_compilation_info() {
	MDXX_thread_safe_print(stdout, (std::string("code-block:\t") + MDXX_COMPILATION_INFO + ".\n").c_str());
}

template<>
const char * mdxx::Expansion<Code_Block*>::to_string() {
	std::stringstream strstr;
	strstr << "Code_Block @ " << data << MDXX_FUNC_COLOR << " (object)" << MDXX_RESET;
	data->code_block_object_id = strstr.str();
	return data->code_block_object_id.c_str();
}
