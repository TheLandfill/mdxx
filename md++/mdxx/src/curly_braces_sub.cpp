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

#include "curly_braces_sub.h"
#include "mdxx_get.h"
#include "re2/re2.h"

namespace mdxx {

const Autosub curly_autosubs[] = {
	{ "!{", "<span class=\"spoiler\">"},
	{"}!", "</span>"},
	{ "v{", "<sub>"},
	{"}v", "</sub>"},
	{ "^{", "<sup>"},
	{"}^", "</sup>"},
	{ "~{", "<del>"},
	{"}~", "</del>"},
	{ "i{", "<em>"},
	{"}i", "</em>"},
	{ "b{", "<b>"},
	{"}b", "</b>"},
	{ "B{", "<b><em>"},
	{"}B", "</em></b>"},
	{ "q{", "<q>"},
	{"}q", "</q>"},
	{ "Q{", "<blockquote>"},
	{"}Q", "</blockquote>"},
	{ "c{", "<code>"},
	{"}c", "</code>"},
	{ "1{", "<h1>", true},
	{"}1", "</h1>", true},
	{ "2{", "<h2>", true},
	{"}2", "</h2>", true},
	{ "3{", "<h3>", true},
	{"}3", "</h3>", true},
	{ "4{", "<h4>", true},
	{"}4", "</h4>", true},
	{ "5{", "<h5>", true},
	{"}5", "</h5>", true},
	{ "6{", "<h6>", true},
	{"}6", "</h6>", true},
	{"{", "{"},
	{"}", "}"},
	{ "\\{", "{"},
	{"}\\",  "}"},
	{nullptr, nullptr}
};

void handle_autosubs(std::string* line, const RE2& r, HTML_Manager& html, MDXX_Manager& mdxx) {
	re2::StringPiece current_sub;
	re2::StringPiece cur_line = *line;
	while (RE2::PartialMatch(cur_line, r, &current_sub)) {
		if (current_sub.as_string().find_first_of("123456") != std::string::npos) {
			html.check_and_close_paragraph();
		}
		RE2::Replace(line, r, MDXX_GET(const char *, mdxx.get_var(current_sub.as_string())));
		cur_line = *line;
	}
}

void handle_curly_braces(std::string* line, HTML_Manager& html, MDXX_Manager& mdxx) {
	static const RE2 left_bracket_at_start_of_line("^{");
	RE2::Replace(line, left_bracket_at_start_of_line, MDXX_GET(const char *, mdxx.get_var("{")));
	static const RE2 left_bracket("([^\\\\]){");
	RE2::GlobalReplace(line, left_bracket, std::string("\\1") + MDXX_GET(const char *, mdxx.get_var("{")));
	static const RE2 right_bracket("([^\\\\])}");
	RE2::GlobalReplace(line, right_bracket, std::string("\\1") + MDXX_GET(const char *, mdxx.get_var("}")));
	static const RE2 left_autosub("([!v^~ibBqQc1-6]{)");
	static const RE2 right_autosub("(}[!v^~ibBqQc1-6])");
	handle_autosubs(line, left_autosub, html, mdxx);
	handle_autosubs(line, right_autosub, html, mdxx);
	static const RE2 escaped_left_bracket("\\\\{");
	RE2::GlobalReplace(line, escaped_left_bracket, MDXX_GET(const char *, mdxx.get_var("\\{")));
	static const RE2 escaped_right_bracket("}\\\\");
	RE2::GlobalReplace(line, escaped_right_bracket, MDXX_GET(const char *, mdxx.get_var("}\\")));
}

}
