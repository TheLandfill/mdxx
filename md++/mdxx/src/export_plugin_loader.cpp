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

#include "dll_info.h"
#include "plugin_loader.h"
#include "expansion.h"
#include "mdxx_get.h"
#include <iostream>

namespace mdxx {

char * MDXX_load_plugins(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr <<
"MDXX_load_plugins expects at least one argument with the rest of the arguments\n"
"being the names of the plugins you want to load as text. The first\n"
"argument should be a Plugin_Loader* (should be {{plugin-obj}})."
		<< std::endl;
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	Plugin_Loader * pl = MDXX_GET(Plugin_Loader*, args[0]);
	for (size_t i = 1; i < argc; i++) {
		const char * plugin_name = MDXX_GET(const char *, args[i]);
		pl->load_plugin(mdxx, plugin_name);
	}
	return nullptr;
}

}
