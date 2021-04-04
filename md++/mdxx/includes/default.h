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

#ifndef MDXX_DEFAULT_H
#define MDXX_DEFAULT_H
#include "context.h"
#include <memory>
#include <unordered_map>

namespace mdxx {

class Default : public Context {
public:
	Default(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~Default();
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

extern std::string empty_str;

}

#endif
