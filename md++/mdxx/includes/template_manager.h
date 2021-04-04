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

#ifndef MDXX_TEMPLATE_MANAGER_H
#define MDXX_TEMPLATE_MANAGER_H
#include "html_manager.h"
#include "mdxx_manager.h"
#include "content_manager.h"
#include "plugin_loader.h"
#include "template_context.h"
#include <memory>

namespace mdxx {

class Template_Manager {
public:
	Template_Manager(HTML_Manager& html, std::shared_ptr<Content_Manager> content, std::string template_file);
	void process_template();
	bool had_error();
public:
	std::string template_object_id;
private:
	HTML_Manager& html;
	std::shared_ptr<Content_Manager> content;
	MDXX_Manager& mdxx;
	std::string template_name;
	MDXX_Manager template_mdxx;
};

template<>
const char * Expansion<Template_Manager*>::to_string();

}

#endif
