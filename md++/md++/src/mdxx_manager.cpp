#include "mdxx_manager.h"
#include "expansion.h"
#include "html_manager.h"
#include "split.h"
#include "re2/re2.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <unordered_map>

namespace mdxx {

std::unordered_map<std::string, std::unique_ptr<Context>> MDXX_Manager::context_dict;
std::unordered_map<std::string, gen_func> MDXX_Manager::imported_function_dict;

MDXX_Manager::MDXX_Manager(std::ifstream& i) : in(i) {}

MDXX_Manager::MDXX_Manager(std::ifstream&& i) : in(i) {}

void MDXX_Manager::print_expansion_flip() {
	print_expansion = !print_expansion;
}

const std::vector<std::string>& MDXX_Manager::context_stack() const {
	return context;
}


std::unique_ptr<Context>& MDXX_Manager::cur_context() {
	return context_dict.at(context.back());
}

variable_map& MDXX_Manager::cur_context_vars() {
	return cur_context()->get_variables();
}

std::string MDXX_Manager::next_line() {
	line_number += 1;
	std::string current_line;
	std::getline(in, current_line);
	return current_line;
}

std::string MDXX_Manager::next_line_no_nl() {
	std::string current_line = next_line();
	while (current_line.back() == '\n' || current_line.back() == '\r') {
		current_line.pop_back();
	}
	return current_line;
}

std::string MDXX_Manager::print_line() {
	std::stringstream strstr;
	strstr << "\tLine " << std::setfill('0') << std::setw(5) << line_number;
	strstr << ":\t" << line_data.line << "\n";
	return strstr.str();
}

void MDXX_Manager::handle_context(HTML_Manager& html) {
	using namespace re2;
	std::string line = line_data.line;
	static const RE2 variable_definition_regex("^\\{\\{.+\\}\\}:=\".+\"$");
	static const RE2 function_import_regex("^\\{\\{.+?\\}\\}:=\\{\\{import\\s+.+\\}\\}");
	static const RE2 immediate_substitution_regex("^\\{\\{.+\\}\\}=\".+\"$");
	static const RE2 open_context_regex("^o\\{\\{.+\\}\\}$");
	static const RE2 close_context_regex("^c\\{\\{.+\\}\\}$");
	if (RE2::FullMatch(line, variable_definition_regex)) {
		variable_definition(line);
	} else if (RE2::FullMatch(line, function_import_regex)) {
		function_import(line);
	} else if (RE2::FullMatch(line, immediate_substitution_regex)) {
		immediate_substitution(line);
	} else if (RE2::FullMatch(line, open_context_regex)) {
		open_context(line, html);
	} else if (RE2::FullMatch(line, close_context_regex)) {
		close_context(line, html);
	} else {
		line_data.line = expand_line(line_data.line);
		cur_context()->process(html, line_data);
	}
}

void MDXX_Manager::variable_definition(std::string& line) {
	size_t variable_end = line.find("}}:=\"");
	size_t variable_start = sizeof("{{");
	size_t value_start = variable_end + sizeof("}}:=\"");
	std::string variable = line.substr(variable_start, variable_end - variable_start);
	std::string value = line.substr(value_start, line.length() - value_start - sizeof("}}"));
	cur_context_vars().insert({variable, MAKE_EXPANSION(std::string, value)});
}

void MDXX_Manager::function_import(std::string& line) {
	size_t variable_end = line.find("}}:={{import ");
	size_t variable_start = sizeof("{{");
	size_t value_start = line.find_first_not_of(" \t", variable_end + sizeof("}}:={{import "));
	std::string variable = line.substr(variable_start, variable_end - variable_start);
	std::string value_id = line.substr(value_start, line.length() - value_start - sizeof("}}"));
	gen_func value = imported_function_dict.at(value_id);
	cur_context_vars().insert({variable, MAKE_GEN_FUNC_EXPANSION(value)});
}

void MDXX_Manager::immediate_substitution(std::string& line) {
	size_t variable_end = line.find("}}=\"");
	size_t variable_start = sizeof("{{");
	size_t value_start = variable_end + sizeof("}}=\"");
	std::string variable = line.substr(variable_start, variable_end - variable_start);
	std::string value = line.substr(value_start, line.length() - value_start - sizeof("\""));
	std::string expanded_value = expand_line(value);
	cur_context_vars().insert({variable, MAKE_EXPANSION(std::string, value)});
}

void MDXX_Manager::open_context(std::string& line, HTML_Manager& html) {
	using namespace re2;
	static const RE2 open_context_regex("[\\{}/]");
	RE2::Replace(&line, open_context_regex, "");
	std::string args = "";
	size_t args_split = line.find_first_not_of("\t ");
	if (args_split != std::string::npos) {
		args = line.substr(args_split + 1);
		line = line.substr(0, args_split);
	}
	if (context_dict.count(line)) {
		context.push_back(line);
		html.check_and_close_paragraph();
		context_dict[line]->open(html, args, *this);
	} else {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += line;
		error_message += " is not a specified context.\nCurrent available contexts:\n";
		for (auto available_context = context_dict.begin(); available_context != context_dict.end(); available_context++) {
			error_message += "\t";
			error_message += available_context->first;
			error_message += "\n";
		}
		error_message += "\nLine that caused the problem:\n";
		error_message += print_line();
		throw std::runtime_error(error_message);
	}
}

void MDXX_Manager::close_context(std::string& line, HTML_Manager& html) {
	static const RE2 close_context_regex("[\\{}/]");
	RE2::Replace(&line, close_context_regex, "");
	if (line == context.back()) {
		cur_context()->close(html);
		context.pop_back();
	} else {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += line;
		error_message += " was not opened but it was closed.";
		error_message += "\nLine that caused the problem:\n";
		error_message += print_line();
		throw std::runtime_error(error_message);
	}
}

void MDXX_Manager::find_next_content_line() {
	size_t count = 0;
	std::string line = "    ";
	if (line_stack != "") {
		count = line_stack.find_first_not_of("\n");
		line_stack = line_stack.substr(count);
		size_t next_line_in_stack = line_stack.find('\n');
		line_data = { line_stack.substr(0, next_line_in_stack), count };
		if (next_line_in_stack == std::string::npos) {
			line_stack = "";
		} else {
			line_stack = line_stack.substr(next_line_in_stack + 1);
		}
		return;
	}
	static const re2::RE2 empty_line_regex("^\\s*$");
	while (re2::RE2::FullMatch(line, empty_line_regex)) {
		count += 1;
		line = next_line();
	}
	while (line.back() == '\n') {
		line.pop_back();
	}
	line_data = Line_Data{ line, count };
}

std::string MDXX_Manager::find_and_return_next_content_line() {
	find_next_content_line();
	return line_data.line;
}

std::string MDXX_Manager::expand_line(std::string & line) {
	std::string complete_line = line + "\n-->\n";
	variable_map& context_vars = cur_context_vars();
	static const RE2 variable_regex("\\{\\{([^{]+?)\\}\\}");
	re2::StringPiece current_sub;
	re2::StringPiece re_line = line;
	while (RE2::FindAndConsume(&re_line, variable_regex, &current_sub)) {
		if (print_expansion) {
			std::cout << complete_line << line << "\n";
		}
		std::vector<std::string> var_args = split(current_sub.as_string());
		std::string var = var_args.front();
		std::vector<std::unique_ptr<Expansion_Base>> args;
		args.reserve(var_args.size());
		for (auto i = var_args.begin() + 1; i != var_args.end(); i++) {
			if (i->front() == '(' && i->back() == ')') {
				std::string current_arg = i->substr(0, i->length() - 1);
				throw_exception_if_variable_not_found(current_arg);
				args.push_back(context_vars[current_arg]->make_deep_copy());
			} else {
				args.push_back(MAKE_EXPANSION(std::string, *i));
			}
		}
		throw_exception_if_variable_not_found(var);
		std::unique_ptr<Expansion_Base>& expanded_var = context_vars[var];
		Expansion<gen_func>* func_holder = dynamic_cast<Expansion<gen_func>*>(expanded_var.get());
		if (func_holder == nullptr) {
			size_t replace_start = current_sub.begin() - re_line.begin();
			size_t replace_end = current_sub.end() - re_line.begin();
			line = line.replace(replace_start, replace_end, *(std::string*)expanded_var->get_data());
		} else {
			size_t replace_start = current_sub.begin() - re_line.begin();
			size_t replace_end = current_sub.end() - re_line.begin();
			line = line.replace(replace_start, replace_end, func_holder->func(args));
		}
		re_line = line;
	}
	size_t line_split = line.find("\n");
	if (line_split != std::string::npos) {
		std::string temp_line = line.substr(0, line_split);
		line_stack = line.substr(line_split + 1) + line_stack;
	}
	static const RE2 left_bracket("(?<!\\\\){");
	RE2::Replace(&line, left_bracket, *(std::string*)context_vars["{"]->get_data());
	static const RE2 right_bracket("(?<!\\\\)}");
	RE2::Replace(&line, right_bracket, *(std::string*)context_vars["}"]->get_data());
	static const RE2 escaped_left_bracket("\\{");
	RE2::Replace(&line, escaped_left_bracket, *(std::string*)context_vars["\\{"]->get_data());
	static const RE2 escaped_right_bracket("\\}");
	RE2::Replace(&line, escaped_right_bracket, *(std::string*)context_vars["\\}"]->get_data());
	if (print_expansion) {
		std::cout << complete_line << line << "\n";
	}
	return line;
}

void MDXX_Manager::set_context(std::vector<std::string> new_context) {
	context = new_context;
}

void MDXX_Manager::throw_exception_if_variable_not_found(const std::string& var) {
	variable_map& context_vars = cur_context_vars();
	if (context_vars.count(var) == 0) {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += var;
		error_message += " is not a variable in the current context.\n";
		error_message += "Current context: ";
		error_message += context.back();
		error_message += "\n";
		for (auto& vars_in_context : context_vars) {
			error_message += "\t";
			error_message += vars_in_context.first;
			error_message += "  -->  ";
			error_message += vars_in_context.second->to_string();
			error_message += "\n";
		}
		error_message += "\nLine that caused the problem:\n";
		error_message += print_line();
		throw std::runtime_error(error_message);
	}
}

void MDXX_Manager::throw_exception_if_context_not_found(const std::string& con) {
	if (context_dict.count(con) == 0) {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += con;
		error_message += " is not a defined context.\n\nValid Contexts:\n";
		for (auto& vars_in_context : context_dict) {
			error_message += "\t";
			error_message += vars_in_context.first;
			error_message += "\n";
		}
		error_message += "\nLine that caused the problem:\n";
		error_message += print_line();
		throw std::runtime_error(error_message);
	}
}

}
