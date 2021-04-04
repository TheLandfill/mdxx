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

#ifndef MDXX_HTML_UTILS_H
#define MDXX_HTML_UTILS_H
#include "expansion.h"
#include "mdxx_manager.h"
#include <vector>
#include <unordered_map>

namespace mdxx {

char * link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img_link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

typedef std::unordered_map<std::string, std::string> str_map;

struct Basic_String {
	char * str;
	size_t length;
};

class Tag_Handler {
public:
	Tag_Handler(
		MDXX_Manager * md,
		Expansion_Base** args,
		size_t argc,
		const char * tag,
		const std::vector<std::string>& attribute_list,
		const std::vector<std::string>& required_url_attribute_list
	);
	Basic_String generate_tag(bool use_url_as_text = false);
private:
	bool catch_bad_urls(std::string url);
	std::vector<std::string> get_url_attribute_value_list();
	std::vector<std::string> organize_user_input();
	str_map create_attribute_map();
private:
	MDXX_Manager* md;
	Expansion_Base** args;
	size_t argc;
	const char * tag;
	std::vector<std::string> attribute_list;
	std::vector<std::string> required_url_attribute_list;
};

}

#endif
