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

#include "html_utils.h"
#include "expansion.h"
#include "join.h"
#include "c_string_copy.h"
#include "sanitize_user_input.h"
#include "mdxx_get.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "thread_safe_print.h"
#include "tag_handler.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include <iostream>
#include <unordered_map>

namespace mdxx {

// First argument is the HTML_Manager*, second could be the inline, then href, then src
char * img_link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	static const std::string inline_check = "inline";
	size_t args_offset = 2;
	if (argc > 2 && inline_check != MDXX_GET(const char *, args[1])) {
		HTML_Manager * html = MDXX_GET(HTML_Manager*, args[0]);
		html->check_and_close_paragraph();
		args_offset -= 1;
	}
	if (args_offset >= argc) {
		std::string error_message;
		error_message.reserve(1024);
		error_message += "Not enough arguments provided to img-a. Arguments provided were\n\n";
		for (size_t i = 0; i < argc; i++) {
			error_message += "\t";
			error_message += args[i]->to_string();
			error_message += "\n";
		}
		MDXX_error(mdxx, error_message.c_str());
		return nullptr;
	}
	Tag_Handler tl{args + args_offset, 1, "a", {""}, {"href"}};
	Basic_String link_part = tl.generate_tag();
	args_offset += 1;
	Tag_Handler t{
		args + args_offset,
		argc - args_offset,
		"img",
		{"alt", "title", "id", "class"},
		{"src"},
		{"width", "height"}
	};
	Basic_String image_part = t.generate_tag();
	std::string output;
	output.reserve(2 * (link_part.length + image_part.length + 5));
	output += link_part.str;
	output += image_part.str;
	output += "</a>";
	return c_string_copy(output);
}

char * img(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	static const std::string inline_check = "inline";
	size_t args_offset = 2;
	if (argc > 2 && inline_check != MDXX_GET(const char *, args[1])) {
		HTML_Manager * html = MDXX_GET(HTML_Manager*, args[0]);
		html->check_and_close_paragraph();
		args_offset -= 1;
	}
	Tag_Handler t{
		args + args_offset,
		argc - args_offset,
		"img",
		{"alt", "title", "id", "class"},
		{"src"},
		{"width", "height"}
	};
	return t.generate_tag().str;
}

char * link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	Tag_Handler t{
		args,
		argc,
		"a",
		{"text", "title", "alt", "id", "class"},
		{"href"}
	};
	return t.generate_tag(true).str;
}

}
