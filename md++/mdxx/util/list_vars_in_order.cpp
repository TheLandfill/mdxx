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

#include "context.h"
#include "mdxx_ansi.h"
#include <algorithm>

namespace mdxx {

std::string unescape_cpp_special_chars(const std::string& str) {
	std::string output;
	output.reserve(2 * str.length());
	for (char c : str) {
		switch (c) {
			case '\a':
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
			case '\?':
			output += "\x1b[38;2;197;214;137m";
		}
		switch (c) {
			case '\a':		output += "\\a"; break;
			case '\b':		output += "\\b"; break;
			case '\f':		output += "\\f"; break;
			case '\n':		output += "\\n"; break;
			case '\r':		output += "\\r"; break;
			case '\t':		output += "\\t"; break;
			case '\v':		output += "\\v"; break;
			case '\?':		output += "\\\?"; break;
			default:		output += c;
		}
		switch (c) {
			case '\a':
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
			case '\?':
			output += MDXX_VAL_COLOR;
		}
	}
	return output;
}

}

const char * MDXX_list_vars(mdxx::variable_map* variables, std::string& all_vars_as_text) {
	typedef std::pair<std::string, mdxx::Expansion_Base*> var_map_item;
	std::vector<var_map_item> vars_in_order;
	vars_in_order.reserve(variables->size());
	for (auto& vars_in_context : *variables) {
		vars_in_order.emplace_back(vars_in_context.first, vars_in_context.second.get());
	}
	std::sort(vars_in_order.begin(), vars_in_order.end(), [](const var_map_item& a, const var_map_item& b){ return a.first < b.first; });
	all_vars_as_text.clear();
	all_vars_as_text += MDXX_BOLD;
	all_vars_as_text += MDXX_VAR_COLOR;
	all_vars_as_text += "           Variable            ";
	all_vars_as_text += MDXX_RESET;
	all_vars_as_text += "┃                        ";
	all_vars_as_text += MDXX_BOLD;
	all_vars_as_text += MDXX_VAL_COLOR;
	all_vars_as_text += "Value\n";
	all_vars_as_text += MDXX_RESET;
	all_vars_as_text += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	for (auto& vars_in_context : vars_in_order) {
		all_vars_as_text += "    ";
		all_vars_as_text += MDXX_VAR_COLOR;
		all_vars_as_text += vars_in_context.first.substr(0, 22);
		if (vars_in_context.first.length() > 22) {
			all_vars_as_text += "...";
		}
		all_vars_as_text += MDXX_RESET;
		all_vars_as_text += std::string(25 - vars_in_context.first.length(), ' ');
		all_vars_as_text += "  ┃  ";
		all_vars_as_text += "\"";
		all_vars_as_text += MDXX_VAL_COLOR;
		all_vars_as_text += mdxx::unescape_cpp_special_chars(vars_in_context.second->to_string());
		all_vars_as_text += MDXX_RESET;
		all_vars_as_text += "\"";
		all_vars_as_text += "\n";
	}
	return all_vars_as_text.c_str();
}
