#define MDXX_WINDOWS_PLUGIN
#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include "plugin_loader.h"
#include "variable_map.h"
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

class Code_Block;

template<>
const char * mdxx::Expansion<Code_Block*>::to_string();

class Code_Block : public mdxx::Context {
public:
	Code_Block(const char * n, mdxx::Plugin_Loader * p, mdxx::MDXX_Manager * m) : name(n), pl(p), md(m) {
		MDXX_py_init();
		variables = MDXX_get_variable_map(pl, this);
		add_variable("{", "{");
		add_variable("}", "}");
		add_variable("\\}", "\\}");
		add_variable("lt", "<");
		add_variable("gt", ">");
		add_variable("amp", "&");
		PyObject* myModule = PyImport_ImportModule("code_block");
		if (myModule == NULL) {
			std::cerr << "ERROR: Could not load code_block.py\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		pygments_wrapper = PyObject_GetAttrString(myModule,(char*)"hand_code_to_pygments");
		if (pygments_wrapper == NULL) {
			std::cerr << "ERROR: Could not find method \"hand_code_to_pygments\", which shouldn't happen.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
	}

	void open(mdxx::HTML_Manager& html, const char * arg_ptr) override {
		(void)html;
		std::vector<std::string> args = mdxx::split(arg_ptr);
		if (args.size() < 1) {
			std::cerr << "ERROR: Need to specify the programming language for the code-block context." << std::endl;
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
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
			std::cerr << "ERROR: You must specify the desired code style when you first open a code-block.\n"
				<< "Example:\n"
				<< "\t{{open code-block c++ trac}}\n"
				<< "where \"trac\" is the preferred code-style."
				<< std::endl;
			MDXX_print_current_line_and_exit(md);
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
		PyObject* py_lines_to_highlight = PyList_New(lines_to_highlight.size());
		if (py_lines_to_highlight == NULL) {
			std::cerr << "ERROR: Could not create python list.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		for (size_t i = 0; i < lines_to_highlight.size(); i++) {
			PyObject* py_current_line = PyLong_FromUnsignedLong(lines_to_highlight[i]);
			if (py_current_line == NULL) {
				std::cerr << "ERROR: Could not convert lines_to_highlight[" << i << "] to a PyLong\n";
				std::cerr << "\tlines_to_highlight[" << i << "]:\t" << lines_to_highlight[i] << "\n";
				PyErr_Print();
				MDXX_print_current_line_and_exit(md);
			}
			if (PyList_SetItem(py_lines_to_highlight, i, py_current_line) == -1) {
				std::cerr << "ERROR: Tried to insert object at index " << i << ", which is out of bounds for the list.\n";
				std::cerr << "Size of list should be " << lines_to_highlight.size() << ".";
				PyErr_Print();
				MDXX_print_current_line_and_exit(md);
			}
		}
		PyObject* py_code_block = PyUnicode_FromString(code_block.c_str());
		if (py_code_block == NULL) {
			std::cerr << "ERROR: Could not convert code_block to python string.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		PyObject* py_code_language = PyUnicode_FromString(code_language.c_str());
		if (py_code_language == NULL) {
			std::cerr << "ERROR: Could not convert code_language to python string.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		PyObject* py_code_style = PyUnicode_FromString(code_style.c_str());
		if (py_code_style == NULL) {
			std::cerr << "ERROR: Could not convert code_style to python string.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		PyObject* py_add_line_numbers = PyBool_FromLong(add_line_numbers);
		if (py_add_line_numbers == NULL) {
			std::cerr << "ERROR: Could not convert add_line_numbers to python bool.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		PyObject* args = PyTuple_Pack(5, py_code_block, py_code_language, py_add_line_numbers, py_code_style, py_lines_to_highlight);
		if (args == NULL) {
			std::cerr << "ERROR: Could not pack the arguments to hand_code_to_pygments.\n";
			std::cerr << "\t" << code_block.c_str()
				<< "\n\t" << code_language.c_str()
				<< "\n\t" << add_line_numbers
				<< "\n\t" << code_style.c_str()
				<< "\n\t" << py_lines_to_highlight
				<< "\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		PyObject* myResult = PyObject_CallObject(pygments_wrapper, args);
		if (myResult == NULL) {
			std::cerr << "ERROR: hand_code_to_pygments was unable to produce output.\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		Py_ssize_t size;
		const char *ptr = PyUnicode_AsUTF8AndSize(myResult, &size);
		if (ptr == NULL) {
			std::cerr << "ERROR: pygments returned something that could not be written in UTF8\n";
			PyErr_Print();
			MDXX_print_current_line_and_exit(md);
		}
		MDXX_html_write(&html, ptr);
		MDXX_html_add(&html, "");
		if (add_line_numbers) {
			MDXX_html_add(&html, "</div>");
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
	PyObject * pygments_wrapper = NULL;
};

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Code_Block)

extern "C" DLL_IMPORT_EXPORT void import_plugin(mdxx::Plugin_Loader * pl, mdxx::MDXX_Manager * mdxx) {
	MDXX_add_new_context(mdxx, "code-block", new Code_Block("code-block", pl, mdxx));
}

extern "C" DLL_IMPORT_EXPORT void print_compilation_info() {
	std::cout << "code-block:\t" << MDXX_COMPILATION_INFO << ".\n";
}

template<>
const char * mdxx::Expansion<Code_Block*>::to_string() {
	std::stringstream strstr;
	strstr << "<Code_Block object @ " << data << ">";
	data->code_block_object_id = strstr.str();
	return data->code_block_object_id.c_str();
}
