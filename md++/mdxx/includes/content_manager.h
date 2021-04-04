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

#ifndef MDXX_CONTENT_MANAGER_H
#define MDXX_CONTENT_MANAGER_H
#include "html_manager.h"
#include "mdxx_manager.h"
#include "expansion.h"
#include <memory>

namespace mdxx {

class Content_Manager {
public:
	Content_Manager(HTML_Manager& html, MDXX_Manager& mdxx, std::string infile);
	void process_content();
	MDXX_Manager& get_mdxx();
	std::string get_infile();
public:
	std::string content_object_id;
private:
	HTML_Manager& html;
	MDXX_Manager& mdxx;
	std::string infile;
};

std::shared_ptr<Content_Manager> get_content_manager(HTML_Manager& html, MDXX_Manager& mdxx);

//template<>
//const char * Expansion<std::shared_ptr<Content_Manager>>::to_string();

template<>
const char * Expansion<Content_Manager*>::to_string();

}

#endif
