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

#include "content_manager.h"
#include "sanitize_user_input.h"
#include "clear_line.h"
#include "thread_safe_print.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

namespace mdxx {

Content_Manager::Content_Manager(HTML_Manager& h, MDXX_Manager& m, std::string in) : html(h), mdxx(m), infile(in) {
	mdxx.set_context({ "default" });
}

void Content_Manager::process_content() {
	try {
		while (true) {
			mdxx.find_next_content_line();
			mdxx.replace_angle_brackets_in_line();
			mdxx.sanitize_ampersands();
			if (mdxx.at_end_of_file()) {
				break;
			}
			mdxx.handle_context(html);
		}
	} catch (std::runtime_error& e) {
		MDXX_error(&mdxx, e.what());
	}
	html.check_and_close_paragraph();
}

MDXX_Manager& Content_Manager::get_mdxx() {
	return mdxx;
}

std::string Content_Manager::get_infile() {
	return infile;
}

template<>
const char * Expansion<Content_Manager*>::to_string() {
	std::stringstream output;
	output << "Content_Manager @ " << data << MDXX_FUNC_COLOR << " (object)" << MDXX_RESET;
	data->content_object_id = output.str();
	return data->content_object_id.c_str();
}

}
