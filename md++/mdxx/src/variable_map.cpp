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
#include "variable_map.h"
#include "plugin_loader.h"
#include "variable_map_definition.h"

using namespace mdxx;

extern "C" {

void MDXX_add_general_variable(variable_map * variables, const char * variable, Expansion_Base * value) {
	std::unique_ptr<Expansion_Base> temp_value(value);
	(*variables)[std::string(variable)] = std::move(temp_value);
}

void MDXX_add_function_variable(variable_map * variables, const char * variable, gen_func func) {
	(*variables)[std::string(variable)] = std::make_unique<Expansion<gen_func>>(func, std::string(variable));
}

void MDXX_add_string_variable(variable_map * variables, const char * variable, const char * value) {
	(*variables)[std::string(variable)] = std::make_unique<Expansion<const char *>>(value);
}

Expansion_Base * MDXX_get_variable(variable_map * variables, const char * variable) {
	return variables->at(std::string(variable)).get();
}

bool MDXX_check_if_var_exists(variable_map * variables, const char * variable) {
	return variables->count(std::string(variable)) > 0;
}

variable_map * MDXX_get_variable_map(Plugin_Loader * pl, void * id) {
	return pl->get_variable_map(id);
}

void MDXX_free_variable_map(Plugin_Loader * pl, void * id) {
	pl->free_variable_map(id);
}

}
