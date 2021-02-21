#include "default.h"
#include "mdxx_manager.h"
#include "content_manager.h"
#include "html_manager.h"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <iostream>

namespace mdxx {

std::string empty_str = "\u0007";

char * print_expansion(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_expansion requires an argument of Expansion<Content_Manager**>.\n"
		);
	}
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args[0]->get_data()))->get_mdxx();
	mdxx.print_expansion_flip();
	return nullptr;
}

char * print_variables(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_variables requires an argument of Expansion<Content_Manager**>.\n"
		);
	}
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args[0]->get_data()))->get_mdxx();
	std::cout << mdxx.list_all_vars() << "\n";
	return nullptr;
}

char * print_context(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_context requires an argument of Expansion<Content_Manager**>.\n"
		);
	}
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args[0]->get_data()))->get_mdxx();
	std::cout << mdxx.list_context_stack() << "\n";
	std::cout << mdxx.list_valid_contexts() << "\n";
	return nullptr;
}

char * print_imported_functions(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_imported_functions requires an argument of Expansion<Content_Manager**>.\n"
		);
	}
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args[0]->get_data()))->get_mdxx();
	std::cout << mdxx.list_imported_functions() << "\n";
	return nullptr;
}

Default::Default(const char * n) : name(n) {
	add_variable("empty", empty_str.c_str());
	add_variable("zs", "&#8203;");
	add_variable("{", "<code>");
	add_variable("}", "</code>");
	add_variable("\\{", "{" );
	add_variable("\\}", "}");
	add_variable("ldb", "\\\\{{{empty}}\\\\{");
	add_variable("rdb", "\\\\}{{empty}}\\\\}");
	add_variable("lt", "&lt;");
	add_variable("gt", "&gt;");
	add_variable("nl", "\n");
	add_variable("print-expansion", print_expansion);
	add_variable("print-variables", print_variables);
	add_variable("print-context", print_context);
	add_variable("print-imported-functions", print_imported_functions);
	add_variable("print", "{{print-expansion (content)}}");
	add_variable("print-vars", "{{print-variables (content)}}");
	add_variable("print-con", "{{print-context (content)}}");
	add_variable("print-func", "{{print-imported-functions (content)}}");
	add_variable("pop", MDXX_html_pop);
	add_variable("push", MDXX_html_push);
	add_variable("mdpu", "{{push (html)}}");
	add_variable("mdpo", "{{pop (html)}}");
}

void throw_default_context_exception() {
	std::string error_message =
"You can neither open nor close the default context manually. You must create\n"
"your own or use one that already exists.";
}

void Default::open(HTML_Manager& html, const char * args) {
	(void)html;
	(void)args;
	throw_default_context_exception();
}

void Default::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	std::string input_line(line_ptr);
	bool blank_lines = num_lines > 1;
	if (input_line == "") {
		return;
	}
	if (blank_lines) {
		html.check_and_close_paragraph();
		std::string line = "<p>";
		line += input_line;
		html.add(line);
		html.push();
		html.open_paragraph();
	} else {
		html.add(input_line);
	}
}

void Default::close(HTML_Manager& html) {
	(void)html;
	throw_default_context_exception();
}

Default::~Default() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Default)

}
