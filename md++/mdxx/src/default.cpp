#include "default.h"
#include "mdxx_manager.h"
#include "content_manager.h"
#include "html_manager.h"
#include "variable_map.h"
#include "html_utils.h"
#include "curly_braces_sub.h"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <iostream>

namespace mdxx {

char * print_expansion(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_expansion requires an argument of MDXX_Manager*.\n"
		);
	}
	MDXX_Manager* mdxx = (*static_cast<MDXX_Manager**>(args[0]->get_data()));
	mdxx->print_expansion_flip();
	return nullptr;
}

char * print_variables(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_variables requires an argument of MDXX_Manager*.\n"
		);
	}
	MDXX_Manager* mdxx = (*static_cast<MDXX_Manager**>(args[0]->get_data()));
	std::cout << mdxx->list_all_vars() << "\n";
	return nullptr;
}

char * print_context(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"print_context requires an argument of MDXX_Manager*.\n"
		);
	}
	MDXX_Manager* mdxx = (*static_cast<MDXX_Manager**>(args[0]->get_data()));
	std::cout << mdxx->list_context_stack() << "\n";
	std::cout << mdxx->list_valid_contexts() << "\n";
	return nullptr;
}

Default::Default(const char * n) : name(n), allow_autosubs(true) {
	add_variable("empty", empty_str.c_str());
	add_variable("zs", "&#8203;");
	add_variable("lt", "&lt;");
	add_variable("gt", "&gt;");
	add_variable("amp", "&amp;");
	add_variable("nl", "\n");
	add_variable("print-expansion", print_expansion);
	add_variable("print-variables", print_variables);
	add_variable("print-context", print_context);
	add_variable("print", "{{print-expansion (mdxx)}}");
	add_variable("print-vars", "{{print-variables (mdxx)}}");
	add_variable("print-con", "{{print-context (mdxx)}}");
	add_variable("pop", MDXX_html_pop);
	add_variable("push", MDXX_html_push);
	add_variable("mdpu", "{{push (html)}}");
	add_variable("mdpo", "{{pop (html)}}");
	add_variable("link-func", link);
	add_variable("img-func", img);
	add_variable("img-a-func", img_link);
	add_variable("a", "{{link-func [1:]}}");
	add_variable("img", "{{img-func (html) [1:]}}");
	add_variable("img-a", "{{img-a-func (html) [1:]}}");
	const Autosub * a = curly_autosubs;
	while (a->var != nullptr) {
		add_variable(a->var, a->value);
		a++;
	}
}

void throw_default_context_exception() {
	std::string error_message =
"You can neither open nor close the default context manually. You must create\n"
"your own or use one that already exists.";
	throw std::runtime_error(error_message);
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
