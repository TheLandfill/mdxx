// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "sanitize_user_input.h"
#include "re2/re2.h"

namespace mdxx {

std::string replace_angle_brackets(const std::string& str) {
	std::string output;
	output.reserve(str.length() * 4);
	for (char c : str) {
		switch (c) {
		case '<':
			output += "{{lt}}";
			break;
		case '>':
			output += "{{gt}}";
			break;
		default:
			output += c;
		}
	}
	return output;
}

void remove_angle_brackets(std::string& str) {
	size_t index = 0;
	for (char c : str) {
		bool is_left = (c == '<');
		bool is_right = (c == '>');
		str[index] = c;
		index += !(is_left || is_right);
	}
	str.erase(index);
}

std::string replace_double_quotes(const std::string& str) {
	std::string output;
	output.reserve(str.length() * 2);
	for (char c : str) {
		if (c == '"') {
			output += "&quot;";
		} else {
			output += c;
		}
	}
	return output;
}

void remove_double_quotes(std::string& str) {
	size_t index = 0;
	for (char c : str) {
		str[index] = c;
		index += (c != '"');
	}
	str.erase(index);
}

std::string replace_quotes(const std::string& str) {
	std::string output;
	output.reserve(str.length() * 2);
	for (char c : str) {
		switch (c) {
		case '"':
			output += "&quot;";
			break;
		case '\'':
			output += "&apos;";
			break;
		default:
			output += c;
		}
	}
	return output;
}

void remove_quotes(std::string& str) {
	size_t index = 0;
	for (char c : str) {
		str[index] = c;
		index += (c != '"' && c != '\'');
	}
	str.erase(index);
}

std::string replace_all_html_chars(const std::string& str) {
	std::string output;
	output.reserve(str.length() * 4);
	for (char c : str) {
		switch (c) {
		case '<':
			output += "{{lt}}";
			break;
		case '>':
			output += "{{gt}}";
			break;
		case '"':
			output += "&quot;";
			break;
		case '\'':
			output += "&apos;";
			break;
		default:
			output += c;
		}
	}
	return output;
}

void remove_all_html_chars(std::string& str) {
	size_t index = 0;
	for (char c : str) {
		bool is_left = (c == '<');
		bool is_right = (c == '>');
		bool is_quot = (c == '"');
		bool is_apos = (c == '\'');
		str[index] = c;
		index += !(is_left || is_right || is_quot || is_apos);
	}
	str.erase(index);
}

// I'm assuming that <> have already been escaped.
void correctly_substitute_ampersands(std::string& str) {
	static const RE2 simple_ampersand_regex("&\\s");
	static const RE2 complex_ampersand_regex_1("&([#]?[a-zA-Z0-9])");
	static const RE2 complex_ampersand_regex_2("<check-amp>([#]?[a-zA-Z0-9]+;)");
	static const RE2 complex_ampersand_regex_3("<check-amp>");
	static const RE2 complex_ampersand_regex_4("<valid-amp>");
	RE2::GlobalReplace(&str, simple_ampersand_regex, "{{amp}} ");
	RE2::GlobalReplace(&str, complex_ampersand_regex_1, "<check-amp>\\1");
	RE2::GlobalReplace(&str, complex_ampersand_regex_2, "<valid-amp>\\1");
	RE2::GlobalReplace(&str, complex_ampersand_regex_3, "{{amp}}");
	RE2::GlobalReplace(&str, complex_ampersand_regex_4, "&");
}

}
