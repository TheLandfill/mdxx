#include "default.h"
#include "mdxx_manager.h"
#include <unordered_map>
#include <iostream>

namespace mdxx {

std::string print_expansion(std::vector<void *>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0));
	mdxx->print_expansion_flip();
	return "";
}

std::string print_variables(std::vector<void *>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0));
	const std::unordered_map<std::string, Expansion>& cur_context_vars = mdxx->cur_context_vars();
	std::cout << "Context Variables: {\n";
	for (auto item : cur_context_vars) {
		std::cout << "\t\"" << item.first << "\", " << item.second.id << "\n";
	}
	std::cout << "}";
	return "";
}

std::string print_context(std::vector<void *>& args) {
	MDXX_Manager* mdxx = static_cast<MDXX_Manager*>(args.at(0));
	std::cout << "Context Stack: [ ";
	auto cur_context = mdxx->cur_context();
	for (auto c : cur_context) {
		std::cout << "\"" << c << "\" ";
	}
	std::cout << "]";
	return "";
}

std::unordered_map<std::string, Expansion> default_dict({
	{ "empty",				{"\u0007"} },
	{ "zs",					{"&#8203;"} },
	{ "{",					{"<code>"} },
	{ "}",					{"</code>"} },
	{ "\\{",				{"{"} },
	{ "\\}",				{"}"} },
	{ "ldb",				{"\\{{{empty}}\\{"} },
	{ "lt",					{"&lt;"} },
	{ "gt",					{"&gt;"} },
	{ "nl",					{"\n"} },
	{ "print-expansion",	{"print_expansion", print_expansion} },
	{ "print",				{"{{print_expansion (self)}}"} },
	{ "print-variables",	{"print_variables", print_variables} },
	{ "print-vars",			{"{{print_variables (self)}}"} },
	{ "print-context",		{"print_context", print_context } },
	{ "print-con",			{"{{print_context (self)}}"} }
});

}
