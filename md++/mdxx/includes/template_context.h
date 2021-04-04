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

#ifndef MDXX_TEMPLATE_CONTEXT_H
#define MDXX_TEMPLATE_CONTEXT_H
#include "context.h"

namespace mdxx {

class Template_Context : public Context {
public:
	Template_Context(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * ls, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~Template_Context();
private:
	bool in_pre_section = false;
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

}

#endif
