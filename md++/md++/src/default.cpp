#include "default.h"
#include "mdxx_manager.h"
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

std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> default_dict;

void fill_default_dict() {
	default_dict.insert({ "empty",				MAKE_EXPANSION(std::string, std::string(empty_str)) });
	default_dict.insert({ "zs",					MAKE_EXPANSION(std::string, "&#8203;") });
	default_dict.insert({ "{",					MAKE_EXPANSION(std::string, "<code>") });
	default_dict.insert({ "}",					MAKE_EXPANSION(std::string, "</code>") });
	default_dict.insert({ "\\{",				MAKE_EXPANSION(std::string, "{") });
	default_dict.insert({ "\\}",				MAKE_EXPANSION(std::string, "}") });
	default_dict.insert({ "ldb",				MAKE_EXPANSION(std::string, "\\{{{empty}}\\{") });
	default_dict.insert({ "lt",					MAKE_EXPANSION(std::string, "&lt;") });
	default_dict.insert({ "gt",					MAKE_EXPANSION(std::string, "&gt;") });
	default_dict.insert({ "nl",					MAKE_EXPANSION(std::string, "\n") });
	default_dict.insert({ "print-expansion",	MAKE_GEN_FUNC_EXPANSION(print_expansion) });
	default_dict.insert({ "print",				MAKE_EXPANSION(std::string, "{{print_expansion (self)}}") });
	default_dict.insert({ "print-variables",	MAKE_GEN_FUNC_EXPANSION(print_variables) });
	default_dict.insert({ "print-vars",			MAKE_EXPANSION(std::string, "{{print_variables (self)}}") });
	default_dict.insert({ "print-context",		MAKE_GEN_FUNC_EXPANSION(print_context) });
	default_dict.insert({ "print-con",			MAKE_EXPANSION(std::string, "{{print_context (self)}}") });
}

}
