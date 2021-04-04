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

#include "html_manager.h"
#include "default.h"
#include "re2/re2.h"
#include "thread_safe_print.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>

namespace mdxx {

std::string empty_str = "\u0007";

HTML_Manager::HTML_Manager(std::string str) : outfile_name(str), out(str) {
	tab_level.reserve(32);
}

void HTML_Manager::add(std::string& line) {
	write(tab_level);
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_pre(std::string& line) {
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_no_nl(std::string& line) {
	write(tab_level);
	write_empty(line);
}

void HTML_Manager::add(const char * line) {
	write(tab_level);
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_pre(const char * line) {
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_no_nl(const char * line) {
	write(tab_level);
	write_empty(line);
}

void HTML_Manager::write(std::string str) {
	if (valid) {
		out << str << std::flush;
	}
}

void HTML_Manager::write_empty(std::string str) {
	remove_empty(str);
	write(str);
}

void HTML_Manager::remove_empty(std::string& str) {
	static const re2::RE2 empty_regex(empty_str);
	re2::RE2::GlobalReplace(&str, empty_regex, "");
}

void HTML_Manager::check_and_close_paragraph() {
	if (need_to_close_paragraph) {
		pop();
		write(tab_level);
		write("</p>\n");
		need_to_close_paragraph = false;
	}
}

void HTML_Manager::push() {
	tab_level += "\t";
}

void HTML_Manager::pop() {
	if (tab_level.length() > 0) {
		tab_level.pop_back();
	} else {
		MDXX_warn(
"You popped the tab_level when it was at zero! Check the html file in\na text "
"editor and see if the indentation is correct.\n"
		);
	}
}

void HTML_Manager::open_paragraph() {
	need_to_close_paragraph = true;
}

void HTML_Manager::delete_outfile() {
	valid = false;
	out.close();
	remove(outfile_name.c_str());
}

template<>
const char * Expansion<HTML_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "HTML_Manager @ "  << this->get_data() << MDXX_FUNC_COLOR << " (object)" << MDXX_RESET;
	data->html_object_id = strstr.str();
	return data->html_object_id.c_str();
}

template<>
Expansion_Base* Expansion<HTML_Manager>::make_deep_copy() {
	throw std::logic_error("HTML_Manager was copied at some point, which should never happen!");
}

}
