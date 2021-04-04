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

#include "html_lists.h"
#include "html_manager.h"
#include "variable_map.h"
#include "mdxx_ansi.h"

namespace mdxx {

HTML_List::HTML_List(const char * n) :
	list_first_element(true),
	name(n),
	allow_autosubs(true)
{}

void HTML_List::open(HTML_Manager& html, const char * args) {
	(void)args;
	std::string opening = std::string("<") + name + ">";
	html.add(opening);
	html.push();
	list_first_element = true;
}

void HTML_List::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	bool non_empty_line = std::string(line_ptr) != "";
	if (list_first_element && non_empty_line) {
		html.add_no_nl("<li>");
		html.add_pre(line_ptr);
		html.push();
		list_first_element = false;
		return;
	}
	if (num_lines > 1) {
		if (non_empty_line) {
			html.pop();
			if (!list_first_element) {
				html.add("</li>");
			}
			html.add_no_nl("<li>");
			html.add_pre(line_ptr);
			html.push();
		}
	} else {
		html.add(line_ptr);
	}
}

void HTML_List::close(HTML_Manager& html) {
	html.pop();
	html.add("</li>");
	html.pop();
	html.add_no_nl("</");
	html.write(name);
	html.add_pre(">");
}

HTML_List::~HTML_List() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(HTML_List)

}
