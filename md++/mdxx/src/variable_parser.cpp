#include "variable_parser.h"
#include "thread_safe_print.h"
#include "mdxx_get.h"
#include "split.h"
#include "str_view_to_int.h"
#include "mdxx_manager.h"
#include <optional>
#include <stdexcept>
#include <iostream>
#include <string>
#include <string_view>

namespace mdxx {

Parser::Parser(MDXX_Manager * m, size_t depth) : max_depth(depth), stack(depth), variable_stack(depth), md(m) {}

Range_Sub_Token::Range_Sub_Token(bool sub, int _start, int _stop) : is_sub(sub), start(_start), stop(_stop) {}

Range_Sub_Token::Range_Sub_Token(bool sub, size_t _begin, size_t _end) : is_sub(sub), begin(_begin), end(_end) {}

bool Parser::push() {
	cur_index++;
	return cur_index < max_depth;
}

bool Parser::pop() {
	bool out = cur_index != 0;
	cur_index -= out;
	return out;
}

std::string Parser::parsing_trace() {
	std::string trace;
	trace.reserve(2048);
	trace += "Parsing Trace:\n";
	for (size_t i = 0; i < max_depth; i++) {
		trace += "\t";
		trace += variable_stack[i];
		trace += "\n\t\t";
		for (const auto& token : cur_depth()) {
			if (token.is_variable) {
				trace += "{{";
			}
			trace += token.str;
			if (token.is_variable) {
				trace += "}}";
			}
		}
	}
	return trace;
}

void Parser::recursion_error() {
	std::string error_message;
	error_message.reserve(1024);
	error_message += "You have exceeded the maximum allowed recursion depth of `";
	error_message += std::to_string(max_depth);
	error_message +=
"` levels of recursion.\n"
"Use substitution (a.k.a. just `=` instead of `:=`) to fix the problem.\n\n";
	error_message += parsing_trace();
	MDXX_error(md, error_message.c_str());
	error_in_parsing = true;
}

std::string Parser::handle_args(const std::string_view& token_str) {
	auto split_args = split(token_str);
	std::string var{ split_args[0] };
	c_args.reserve(split_args.size());
	c_args_to_clean.reserve(split_args.size());
	for (auto i = split_args.begin() + 1; i != split_args.end(); i++) {
		current_arg.clear();
		current_arg.reserve(i->length());
		if ((i->front() == '(') && (i->back() == ')')) {
			current_arg += i->substr(1, i->length() - 2);
			c_args.push_back(md->get_var(current_arg));
		} else {
			char * cur_c_str_arg = new char[i->length() + 1];
			std::copy(i->begin(), i->end(), cur_c_str_arg);
			cur_c_str_arg[i->length()] = '\0';
			c_args_to_clean.emplace_back(new Expansion<char *>(cur_c_str_arg));
			c_args.push_back(c_args_to_clean.back().get());
		}
	}
	std::string arg_value;
	arg_value.reserve(16);
	for (size_t i = 0; i < split_args.size(); i++) {
		arg_value.clear();
		arg_value += '[';
		arg_value += std::to_string(i);
		arg_value += ']';
		cache[arg_value] = split_args[i];
	}
	return var;
}

void Parser::handle_args_subs(const std::string_view& char_line) {
	range_string.clear();
	std::vector<Range_Sub_Token> range_tokens;
	parse_line_for_arg_subs(char_line, range_tokens);
	for (auto token : range_tokens) {
		if (token.is_sub) {
			std::cout << "sub: " << token.start << " - " << token.stop << "\n";
			token.start += (token.start < 0) * c_args.size();
			token.stop += (token.stop < 0) * c_args.size();
			if (
				(size_t)token.start == (c_args.size() + 1)
				|| (size_t)token.stop == (c_args.size() + 1)
			) {
				continue;
			}
			if (
				token.start < 0
				|| token.stop < 0
				|| static_cast<size_t>(token.start) > c_args.size()
				|| static_cast<size_t>(token.stop) > c_args.size()
			) {
				std::string error_message;
				error_message.reserve(2048);
				error_message += "Invalid Range!\n\n";
				error_message += parsing_trace();
				MDXX_error(md, error_message.c_str());
			}
			int step = 1 - 2 * (token.stop < token.start);
			for (int i = token.start; i != token.stop; i += step) {
				range_string += " ";
				range_string += cache[std::string("[") + std::to_string(i) + "]"];
			}
			range_string += " ";
			range_string += cache[std::string("[") + std::to_string(token.stop) + "]"];
		} else {
			std::cout << "no-sub: " << char_line.substr(token.begin, token.end - token.begin) << "\n";
			range_string += char_line.substr(token.begin, token.end - token.begin);
		}
	}
}

void Parser::expand_tokens() {
	for (const auto& token : cur_depth()) {
		token_string.clear();
		token_string += token.str;
		if (token.is_variable && cache.count(token_string) == 0) {
			std::string var{ handle_args(token.str) };
			bool could_push = push();
			variable_stack.push_back(std::string(token.str));
			if (!could_push) {
				recursion_error();
				return;
			}
			Expansion_Base * cur_var = md->get_var(var);
			std::string& cached = cache[token_string];
			cached.clear();
			if (MDXX_is_type<const char *>(cur_var)) {
				const char * str = MDXX_get<const char *>(cur_var);
				handle_args_subs(str);
				std::string temp = range_string;
				sub_parse(temp.c_str(), cached);
			} else if (MDXX_is_type<gen_func>(cur_var)) {
				gen_func func = MDXX_get<gen_func>(cur_var);
				char * function_output = func(md, c_args.data(), c_args.size());
				if (function_output) {
					sub_parse(function_output, cached);
					delete[] function_output;
				}
			}
			pop();
			variable_stack.pop_back();
		}
	}
}

void Parser::reserve_output_length(std::string& str) {
	size_t length = 0;
	for (const auto& token : cur_depth()) {
		if (token.is_variable) {
			token_string.clear();
			token_string = token.str;
			length += cache[token_string].length();
		} else {
			length += token.str.length();
		}
	}
	str.reserve(length + 1);
}

void Parser::fill_output(std::string& output) {
	reserve_output_length(output);
	for (const auto& token : cur_depth()) {
		if (token.is_variable) {
			token_string.clear();
			token_string = token.str;
			output += cache[token_string].c_str();
		} else {
			output += token.str;
		}
	}
}

void Parser::sub_parse(const char * line, std::string& output) {
	if (error_in_parsing) {
		return;
	}
	c_args.clear();
	parse_line_for_variables(md, line, cur_depth());
	expand_tokens();
	fill_output(output);
}

const std::string& Parser::parse(const char * line) {
	cur_index = 0;
	result.clear();
	cache.clear();
	c_args_to_clean.clear();
	sub_parse(line, result);
	return result;
}

void Parser::parse_line_for_arg_subs(const std::string_view& char_line, std::vector<Range_Sub_Token>& output) {
	std::cout << "`" << char_line << "`\n";
	output.clear();
	if (char_line.length() == 0) {
		return;
	}
	std::vector<size_t> token_starts;
	token_starts.reserve(128);
	std::vector<size_t> token_middles;
	token_middles.reserve(128);
	std::vector<size_t> token_ends;
	token_starts.reserve(128);
	for (size_t i = 0; i < char_line.length(); i++) {
		switch (char_line[i]) {
		case '[':
			token_starts.push_back(i);
			break;
		case ':':
			token_middles.push_back(i);
			break;
		case ']':
			token_ends.push_back(i);
			break;
		default:
			;
		}
	}
	size_t cur_location = 0;
	auto cur_start = token_starts.begin();
	auto cur_middle = token_middles.begin();
	auto cur_end = token_ends.begin();
	while (cur_start != token_starts.end()) {
		while (cur_end != token_ends.end() && *cur_end <= *cur_start) {
			++cur_end;
		}
		while (cur_middle != token_middles.end() && *cur_middle <= *cur_start) {
			++cur_middle;
		}
		if (cur_end == token_ends.end()) {
			break;
		}
		std::optional<int> range_start;
		std::optional<int> range_end;
		if (*cur_middle < *cur_end) {
			std::string_view first_part = char_line.substr(*cur_start + 1, *cur_middle - *cur_start - 1);
			if (first_part == "") {
				range_start = 0;
			} else {
				range_start = to_integral_type<int>(first_part);
			}
			std::string_view second_part = char_line.substr(*cur_middle + 1, *cur_end - *cur_middle - 1);
			if (second_part == "") {
				range_end = c_args.size();
			} else {
				range_end = to_integral_type<int>(second_part);
			}
		} else {
			std::string_view part = char_line.substr(*cur_start + 1, *cur_end - *cur_start - 1);
			range_start = to_integral_type<int>(part);
			range_end = *range_start;
		}
		if (range_start != std::nullopt && range_end != std::nullopt) {
			if (cur_location != *cur_start) {
				output.push_back({false, cur_location, *cur_start});
				cur_location = *cur_end + 1;
			}
			output.push_back({true, *range_start, *range_end});
		}
		++cur_start;
	}
	if (cur_location < (char_line.length() - 1)) {
		output.push_back({false, cur_location, char_line.length()});
	}
}

std::vector<MDXX_Token>& Parser::cur_depth() {
	return stack[cur_index];
}

std::string& Parser::cur_variable() {
	return variable_stack[cur_index];
}

void parse_line_for_variables(MDXX_Manager * md, const char * char_line, std::vector<MDXX_Token>& output) {
	output.clear();
	if (char_line[0] == '\0') {
		return;
	} else if (char_line[1] == '\0') {
		output.push_back({{char_line, 1}, false});
	}
	std::vector<size_t> token_starts;
	token_starts.reserve(128);
	std::vector<size_t> token_ends;
	token_starts.reserve(128);
	bool in_var = char_line[0] == '{' && char_line[1] == '{';
	size_t i = 1 + in_var;
	token_starts.push_back(2 * in_var);
	while (char_line[i] != '\0') {
		if (in_var) {
			if (char_line[i] == '{') {
				MDXX_error(md, "You have `{` inside of a variable or a function.");
				return;
			}
			if (char_line[i - 1] == '}' && char_line[i] == '}') {
				token_ends.push_back(i - 2);
				in_var = false;
				if (char_line[i + 1] != '{') {
					token_starts.push_back(i + 1);
				}
			}
		} else if (char_line[i - 1] == '{' && char_line[i] == '{') {
			if (char_line[i - 2] != '}') {
				token_ends.push_back(i - 2);
			}
			token_starts.push_back(i + 1);
			in_var = true;
		}
		i++;
	}
	token_ends.push_back(i);
	i = 0;
	for (size_t j = 0; j < token_starts.size(); j++, i++) {
		if (
			(j < token_starts.size() - 1)
			&& (token_starts[j + 1] < token_ends[i])
		) {
			MDXX_error(md, "You have `{` inside of a variable or a function.");
			return;
		}
		if (char_line[token_starts[j]] == '\0') {
			continue;
		}
		bool is_var =
			(char_line[token_starts[j] - 2] == '{')
			&& (char_line[token_starts[j] - 1] == '{')
			&& (char_line[token_ends[i] + 1] == '}')
			&& (char_line[token_ends[i] + 2] == '}');
		output.push_back({{char_line + token_starts[j], token_ends[i] - token_starts[j] + 1}, is_var});
	}
}

}
