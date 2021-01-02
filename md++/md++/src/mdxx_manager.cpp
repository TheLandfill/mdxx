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
#include <string>
#include <unordered_map>

namespace mdxx {

std::unordered_map<std::string, std::unique_ptr<Context>> MDXX_Manager::context_dict;
std::unordered_map<std::string, gen_func> MDXX_Manager::imported_function_dict;

MDXX_Manager::MDXX_Manager(std::ifstream& i) : in(i) {}

MDXX_Manager::MDXX_Manager(std::ifstream&& i) : in(i) {}

MDXX_Manager::MDXX_Manager(std::string filename) :
	in_if_need_to_allocate(new std::ifstream(filename)),
	in(*in_if_need_to_allocate)
{}

void MDXX_Manager::print_expansion_flip() {
	print_expansion = !print_expansion;
}

const std::vector<std::string>& MDXX_Manager::context_stack() const {
	return context;
}

std::unique_ptr<Context>& MDXX_Manager::cur_context() {
	throw_exception_if_context_not_found(context.back(), this);
	return context_dict.at(context.back());
}

variable_map& MDXX_Manager::cur_context_vars() {
	return cur_context()->get_variables();
}

std::unique_ptr<Expansion_Base>& MDXX_Manager::get_var(std::string variable) {
	std::string context_with_variable = find_context_with_variable(variable);
	return context_dict.at(context_with_variable)->get_variables().at(variable);
}

std::string MDXX_Manager::next_line() {
	line_number += 1;
	if (in.peek() == EOF) {
		line_data.line = "";
		finished_reading = true;
	} else {
		std::getline(in, line_data.line);
	}
	return line_data.line;
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
	static const RE2 variable_definition_regex("^\\{\\{[^{}]+\\}\\}:=\".+\"$");
	//static const RE2 function_import_regex("^\\{\\{[^{}]+?\\}\\}:=\\{\\{import\\s+[^{}]+\\}\\}");
	static const RE2 immediate_substitution_regex("^\\{\\{[^{}]+\\}\\}=\".+\"$");
	if (RE2::FullMatch(line, variable_definition_regex)) {
		variable_definition(line);
	//} else if (RE2::FullMatch(line, function_import_regex)) {
	//	function_import(line);
	} else if (RE2::FullMatch(line, immediate_substitution_regex)) {
		immediate_substitution(line);
	} else {
		line_data.line = expand_line(line);
		cur_context()->process(html, line_data);
	}
}

void MDXX_Manager::variable_definition(std::string& line) {
	size_t variable_end = line.find("}}:=\"");
	size_t variable_start = sizeof("{{") - 1;
	size_t value_start = variable_end + sizeof("}}:=\"") - 1;
	std::string variable = line.substr(variable_start, variable_end - variable_start);
	std::string value = line.substr(value_start, line.length() - value_start - sizeof("\"") + 1);
	cur_context_vars()[variable] = std::make_unique<Expansion<std::string>>(value);
}

//void MDXX_Manager::function_import(std::string& line) {
//	size_t variable_end = line.find("}}:={{import ");
//	size_t variable_start = sizeof("{{") - 1;
//	size_t value_start = line.find_first_not_of(" \t", variable_end + sizeof("}}:={{import ")) - 1;
//	std::string variable = line.substr(variable_start, variable_end - variable_start);
//	std::string value_id = line.substr(value_start, line.length() - value_start - sizeof("}}") + 1);
//	check_if_imported_function_found(value_id);
//	gen_func value = imported_function_dict.at(value_id);
//	cur_context_vars()[variable] = std::make_unique<Expansion<gen_func>>(value, value_id);
//}

void MDXX_Manager::immediate_substitution(std::string& line) {
	size_t variable_end = line.find("}}=\"");
	size_t variable_start = sizeof("{{") - 1;
	size_t value_start = variable_end + sizeof("}}=\"") - 1;
	std::string variable = line.substr(variable_start, variable_end - variable_start);
	std::string value = line.substr(value_start, line.length() - value_start - sizeof("\"") + 1);
	std::string expanded_value = expand_line(value);
	cur_context_vars()[variable] = std::make_unique<Expansion<std::string>>(value);
}

std::string MDXX_Manager::open_context(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	std::string line = *static_cast<std::string*>(args.at(0)->get_data());
	MDXX_Manager& mdxx = **static_cast<MDXX_Manager**>(args.at(1)->get_data());
	HTML_Manager& html = **static_cast<HTML_Manager**>(args.at(2)->get_data());
	using namespace re2;
	static const RE2 open_context_regex("[\\\\{}/]");
	RE2::Replace(&line, open_context_regex, "");
	std::string open_args = "";
	size_t args_split = line.find_first_of("\t ");
	if (args_split != std::string::npos) {
		open_args = line.substr(args_split + 1);
		line = line.substr(0, args_split);
	}
	throw_exception_if_context_not_found(line, &mdxx);
	mdxx.context.push_back(line);
	html.check_and_close_paragraph();
	context_dict[line]->open(html, open_args, mdxx);
	return "";
}

std::string MDXX_Manager::close_context(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	std::string line = *static_cast<std::string*>(args.at(0)->get_data());
	MDXX_Manager& mdxx = **static_cast<MDXX_Manager**>(args.at(1)->get_data());
	HTML_Manager& html = **static_cast<HTML_Manager**>(args.at(2)->get_data());
	static const RE2 close_context_regex("[\\\\{}/]");
	RE2::Replace(&line, close_context_regex, "");
	if (line == mdxx.context.back()) {
		mdxx.cur_context()->close(html);
		mdxx.context.pop_back();
	} else {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += line;
		error_message += " was not opened but it was closed.";
		error_message += "\nLine that caused the problem:\n";
		error_message += mdxx.print_line();
		throw std::runtime_error(error_message);
	}
	return "";
}

void MDXX_Manager::find_next_content_line() {
	size_t count = 0;
	std::string current_line = "    ";
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
	while (!at_end_of_file() && re2::RE2::FullMatch(current_line, empty_line_regex)) {
		count += 1;
		current_line = next_line();
	}
	while (current_line.back() == '\n') {
		current_line.pop_back();
	}
	line_data = Line_Data{ current_line, count };
}

std::string MDXX_Manager::find_and_return_next_content_line() {
	find_next_content_line();
	return line_data.line;
}

std::string MDXX_Manager::expand_line(std::string& line) {
	std::string complete_line = line + " --> ";
	static const RE2 variable_regex("\\{\\{([^{}]+)\\}\\}");
	re2::StringPiece current_sub;
	re2::StringPiece re_line = line;
	while (RE2::PartialMatch(re_line, variable_regex, &current_sub)) {
		if (print_expansion) {
			std::cout << complete_line << line << "\n";
		}
		std::vector<std::string> var_args = split(current_sub.as_string());
		std::string var = var_args.front();
		std::vector<std::unique_ptr<Expansion_Base>> args;
		args.reserve(var_args.size());
		for (auto i = var_args.begin() + 1; i != var_args.end(); i++) {
			if (i->front() == '(' && i->back() == ')') {
				std::string current_arg = i->substr(1, i->length() - 2);
				args.push_back(get_var(current_arg)->make_deep_copy());
			} else {
				args.push_back(std::make_unique<Expansion<std::string>>(*i));
			}
		}
		std::unique_ptr<Expansion_Base>& expanded_var = get_var(var);
		Expansion<gen_func>* func_holder = dynamic_cast<Expansion<gen_func>*>(expanded_var.get());
		if (func_holder == nullptr) {
			RE2::Replace(&line, variable_regex, *static_cast<std::string*>(expanded_var->get_data()));
		} else {
			RE2::Replace(&line, variable_regex, func_holder->func(args));
		}
		re_line = line;
	}
	size_t line_split = line.find("\n");
	if (line_split != std::string::npos) {
		std::string temp_line = line.substr(0, line_split);
		line_stack = line.substr(line_split + 1) + line_stack;
	}
	static const RE2 left_bracket_at_start_of_line("^{");
	RE2::Replace(&line, left_bracket_at_start_of_line, *static_cast<std::string*>(get_var("{")->get_data()));
	static const RE2 left_bracket("([^\\\\]){");
	RE2::GlobalReplace(&line, left_bracket, std::string("\\1") + *static_cast<std::string*>(get_var("{")->get_data()));
	static const RE2 right_bracket("([^\\\\])}");
	RE2::GlobalReplace(&line, right_bracket, std::string("\\1") + *static_cast<std::string*>(get_var("}")->get_data()));
	static const RE2 escaped_left_bracket("\\\\{");
	RE2::GlobalReplace(&line, escaped_left_bracket, *static_cast<std::string*>(get_var("\\{")->get_data()));
	static const RE2 escaped_right_bracket("\\\\}");
	RE2::GlobalReplace(&line, escaped_right_bracket, *static_cast<std::string*>(get_var("\\}")->get_data()));
	if (print_expansion) {
		std::cout << complete_line << line << "\n";
	}
	return line;
}

void MDXX_Manager::set_context(std::vector<std::string> new_context) {
	context = new_context;
}

std::string MDXX_Manager::find_context_with_variable(const std::string& var) {
	for (auto cur_context = context.rbegin(); cur_context != context.rend(); cur_context++) {
		throw_exception_if_context_not_found(*cur_context, this);
		variable_map& context_vars = context_dict.at(*cur_context)->get_variables();
		if (context_vars.count(var) != 0) {
			return *cur_context;
		}
	}
	std::string error_message = "ERROR: ";
	error_message.reserve(2048);
	error_message += var;
	error_message += " is not a variable in the current context stack.\n";
	error_message += list_context_stack();
	error_message += "\n\n";
	error_message += list_all_vars();
	error_message += "\nLine that caused the problem:\n";
	error_message += print_line();
	throw std::runtime_error(error_message);
}

void MDXX_Manager::throw_exception_if_context_not_found(const std::string& con, MDXX_Manager* mdxx) {
	if (context_dict.count(con) == 0) {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += con;
		error_message += " is not a defined context.\n\n";
		error_message += list_valid_contexts();
		if (mdxx != nullptr) {
			error_message += "\nLine that caused the problem:\n";
			error_message += mdxx->print_line();
		}
		throw std::runtime_error(error_message);
	}
}

void MDXX_Manager::check_if_imported_function_found(const std::string& function) {
	if (imported_function_dict.count(function) == 0) {
		std::string error_message = "ERROR: ";
		error_message.reserve(2048);
		error_message += function;
		error_message += " Has not been imported.\n\n";
		error_message += list_imported_functions();
		error_message += "\nLine that caused the problem:\n";
		error_message += print_line();
		throw std::runtime_error(error_message);
	}
}

bool MDXX_Manager::at_end_of_file() {
	return finished_reading;
}

std::string MDXX_Manager::list_all_vars() {
	std::string output;
	for (auto cur_context = context.rbegin(); cur_context != context.rend(); cur_context++) {
		throw_exception_if_context_not_found(*cur_context, this);
		variable_map& context_vars = context_dict.at(*cur_context)->get_variables();
		output += *cur_context;
		output += "\n";
		for (auto& vars_in_context : context_vars) {
			output += "\t";
			output += vars_in_context.first;
			output += "  -->  ";
			output += vars_in_context.second->to_string();
			output += "\n";
		}
	}
	return output;
}

std::string MDXX_Manager::list_context_stack() {
	std::string output = "Current context: { ";
	for (auto cur_context = context.rbegin(); cur_context != context.rend(); cur_context++) {
		output += *cur_context;
		output += " ";
	}
	output += "}";
	return output;
}

template<typename T>
std::string list_elements_in_dict(const std::unordered_map<std::string, T>& dict) {
	std::string output;
	output.reserve(2048);
	for (auto& element : dict) {
		output += "\t";
		output += element.first;
		output += "\n";
	}
	return output;
}

std::string MDXX_Manager::list_valid_contexts() {
	std::string output = "Valid Contexts:\n";
	output += list_elements_in_dict(context_dict);
	return output;
}

std::string MDXX_Manager::list_imported_functions() {
	std::string output = "Imported Functions:\n";
	output += list_elements_in_dict(imported_function_dict);
	return output;
}

MDXX_Manager::~MDXX_Manager() {
	delete in_if_need_to_allocate;
}

template<>
std::string Expansion<MDXX_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<MDXX_Manager object @ " << this->get_data() << ">";
	return strstr.str();
}

}
