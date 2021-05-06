#include "variable_parser.h"
#include "thread_safe_print.h"
#include <stdexcept>
#include <iostream>
#include <string_view>

namespace mdxx {

void parse_line_for_variables(MDXX_Manager * md, const char * char_line, std::vector<MDXX_Token>& output) {
	output.clear();
	std::cout << char_line << "\n";
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
			if ((char_line[i - 1] == '{' && char_line[i] == '{')) {
				MDXX_error(md, "You have `{` inside of a variable or a function.");
				return;
			}
			if ((char_line[i - 1] == '}' && char_line[i] == '}')) {
				token_ends.push_back(i - 2);
				in_var = false;
				if (char_line[i + 1] != '{') {
					token_starts.push_back(i + 1);
				}
			}
		} else if ((char_line[i - 1] == '{' && char_line[i] == '{')) {
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
	for (size_t m = 0; m < token_starts.size(); m++) {
		std::cout << token_starts[m] << ", ";
	}
	std::cout << "\n";
	for (size_t m = 0; m < token_ends.size(); m++) {
		std::cout << token_ends[m] << ", ";
	}
	std::cout << "\n";
	for (size_t j = 0; j < token_starts.size() - 1; j++, i++) {
		if (token_starts[j + 1] < token_ends[i]) {
			MDXX_error(md, "You have `{` inside of a variable or a function.");
			return;
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
