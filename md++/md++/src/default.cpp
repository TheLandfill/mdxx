#include "default.h"
#include "mdxx_manager.h"
#include "content_manager.h"
#include "html_manager.h"
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace mdxx {

std::string empty_str = "\u0007";

std::string print_expansion(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args.at(0)->get_data()))->get_mdxx();
	mdxx.print_expansion_flip();
	return "";
}

std::string print_variables(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args.at(0)->get_data()))->get_mdxx();
	std::cout << mdxx.list_all_vars() << "\n";
	return "";
}

std::string print_context(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args.at(0)->get_data()))->get_mdxx();
	std::cout << mdxx.list_context_stack() << "\n";
	std::cout << mdxx.list_valid_contexts() << "\n";
	return "";
}

std::string print_imported_functions(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager& mdxx = (*static_cast<Content_Manager**>(args.at(0)->get_data()))->get_mdxx();
	std::cout << mdxx.list_imported_functions() << "\n";
	return "";
}

Default::Default(std::string name) : Context(name) {
	add_variable<std::string>("empty", std::string(empty_str) );
	add_variable<std::string>("zs", "&#8203;");
	add_variable<std::string>("{", "<code>");
	add_variable<std::string>("}", "</code>");
	add_variable<std::string>("\\{", "{" );
	add_variable<std::string>("\\}", "}");
	add_variable<std::string>("ldb", "\\{{{empty}}\\{");
	add_variable<std::string>("lt", "&lt;");
	add_variable<std::string>("gt", "&gt;");
	add_variable<std::string>("nl", "\n");
	add_variable<gen_func>("print-expansion", print_expansion);
	add_variable<gen_func>("print-variables", print_variables);
	add_variable<gen_func>("print-context", print_context);
	add_variable<gen_func>("print-imported-functions", print_imported_functions);
	add_variable<std::string>("print", "{{print_expansion (content)}}");
	add_variable<std::string>("print-vars", "{{print-variables (content)}}");
	add_variable<std::string>("print-con", "{{print-context (content)}}");
	add_variable<std::string>("print-func", "{{print-imported-functions (content)}}");
	add_variable<gen_func>("pop", HTML_Manager::pop);
	add_variable<gen_func>("push", HTML_Manager::push);
	add_variable<std::string>("mdpu", "{{push (html)}}");
	add_variable<std::string>("mdpo", "{{pop (html)}}");
}

void throw_default_context_exception() {
	std::string error_message =
"You can neither open nor close the default context manually. You must create\n"
"your own or use one that already exists.";
}

void Default::open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) {
	(void)html;
	(void)args;
	(void)mdxx;
	throw_default_context_exception();
}

void Default::process(HTML_Manager& html, Line_Data& ls) {
	bool blank_lines = ls.num_lines > 1;
	std::cout << "{ " << ls.line << ", " << ls.num_lines << " }\n";
	if (blank_lines) {
		html.check_and_close_paragraph();
		std::string line = "<p>";
		line += ls.line;
		html.add(line);
		html.push();
		html.open_paragraph();
	} else {
		html.add(ls.line);
	}
}

void Default::close(HTML_Manager& html) {
	(void)html;
	throw_default_context_exception();
}

}
