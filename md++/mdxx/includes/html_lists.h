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

#ifndef MDXX_LISTS_H
#define MDXX_LISTS_H
#include "context.h"

namespace mdxx {
class HTML_List : public Context {
public:
	HTML_List(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~HTML_List();
private:
	bool list_first_element;
MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

}

#endif
