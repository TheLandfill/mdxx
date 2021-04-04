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

#ifndef MDXX_METADATA_H
#define MDXX_METADATA_H
#include "mdxx_manager.h"
#include "dll_info.h"
#include <vector>
#include <string>

extern "C" DLL_IMPORT_EXPORT char * MDXX_read_metadata_file(mdxx::MDXX_Manager * md, mdxx::Expansion_Base** args, size_t argc);


#endif
