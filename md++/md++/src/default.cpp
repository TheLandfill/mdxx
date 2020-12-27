#include "default.h"
#include "mdxx_manager.h"
#include "html_manager.h"
#include <memory>
#include <unordered_map>
#include <iostream>

namespace mdxx {

std::string empty_str = "\u0007";

std::string print_expansion(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0)->get_data());
	mdxx->print_expansion_flip();
	return "";
}

std::string print_variables(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0)->get_data());
	const std::unordered_map<std::string, std::unique_ptr<Expansion_Base>>& cur_context_vars = mdxx->cur_context_vars();
	std::cout << "Context Variables: {\n";
	for (auto& item : cur_context_vars) {
		std::cout << "\t\"" << item.first << "\", " << item.second->to_string() << "\n";
	}
	std::cout << "}";
	return "";
}

std::string print_context(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0)->get_data());
	std::cout << "Context Stack: [ ";
	auto& context_stack = mdxx->context_stack();
	for (auto c : context_stack) {
		std::cout << "\"" << c << "\" ";
	}
	std::cout << "]";
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
	add_variable<std::string>("print", "{{print_expansion (self)}}");
	add_variable<std::string>("print-vars", "{{print_variables (self)}}");
	add_variable<std::string>("print-con", "{{print_context (self)}}");
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
	html.check_and_close_paragraph();
}

void Default::close(HTML_Manager& html) {
	(void)html;
	throw_default_context_exception();
}

}
