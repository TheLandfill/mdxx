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
#include <unordered_map>

namespace mdxx {

void remove_chars(std::string& str, const std::unordered_set<char>& rem) {
	size_t index = 0;
	for (char c : str) {
		str[index] = c;
		index += (!rem.count(c));
	}
	str.erase(index);
}

std::string replace_chars(const std::string& str, const std::unordered_map<char, std::string>& rep) {
	std::string output;
	output.reserve(str.length() & 4);
	for (char c : str) {
		if (rep.count(c) > 0) {
			output += rep.at(c);
		} else {
			output += c;
		}
	}
	return output;
}

std::string replace_angle_brackets(const std::string& str) {
	return replace_chars(str, { {'<', "{{lt}}"}, {'>', "{{gt}}"} });
}

void remove_angle_brackets(std::string& str) {
	remove_chars(str, {'<', '>'});
}

std::string replace_double_quotes(const std::string& str) {
	return replace_chars(str, { { '"', "&quot;" } } );
}

void remove_double_quotes(std::string& str) {
	remove_chars(str, {'"'});
}

std::string replace_quotes(const std::string& str) {
	return replace_chars(str, { { '"', "&quot;" }, { '\'', "&apos;" } } );
}

void remove_quotes(std::string& str) {
	remove_chars(str, { '\'', '"' });
}

std::string replace_all_html_chars(const std::string& str) {
	return replace_chars(str, { { '"', "&quot;" }, { '\'', "&apos;" }, {'<', "{{lt}}"}, {'>', "{{gt}}"} } );
}

void remove_all_html_chars(std::string& str) {
	remove_chars(str, {'<', '>', '"', '\''});
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
