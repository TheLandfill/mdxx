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

#include "template_context.h"
#include "expansion.h"
#include "html_manager.h"
#include "process_html.h"
#include "variable_map.h"
#include "mdxx_ansi.h"

namespace mdxx {

Template_Context::Template_Context(const char * n) : name(n) {}

void Template_Context::open(HTML_Manager& html, const char * args) {
	(void)html;
	(void)args;
	return;
}

void Template_Context::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	process_html(html, line_ptr, num_lines, in_pre_section);
}

void Template_Context::close(HTML_Manager& html) {
	(void)html;
	return;
}

Template_Context::~Template_Context() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Template_Context)

}
