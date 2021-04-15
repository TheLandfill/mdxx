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

namespace mdxx {

char * link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img_link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * span(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * div(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

}

#endif
