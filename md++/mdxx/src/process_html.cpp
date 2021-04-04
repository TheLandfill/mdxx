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

#include "process_html.h"

namespace mdxx {

void process_html(HTML_Manager& html, const char * line_ptr, size_t num_lines, bool& in_pre_section) {
	std::string line(line_ptr);
	if (in_pre_section) {
		std::string blank_line = "";
		for (size_t i = 1; i < num_lines; i++) {
			html.add_pre(blank_line);
		}
		html.add_pre(line);
	} else {
		html.add(line);
	}
	if (line.find("<pre>") != std::string::npos) {
		in_pre_section = true;
	}
	if (line.find("</pre>") != std::string::npos) {
		in_pre_section = false;
	}
}

}
