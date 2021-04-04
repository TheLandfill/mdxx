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

#ifndef MDXX_VARIABLE_MAP_H
#define MDXX_VARIABLE_MAP_H
#include "expansion.h"
#include "dll_info.h"
#include "variable_map_definition.h"
#include "plugin_loader.h"
#include <string>
#include <unordered_map>
#include <memory>

extern "C" DLL_IMPORT_EXPORT void MDXX_add_general_variable(mdxx::variable_map * variables, const char * variable, mdxx::Expansion_Base * value);
extern "C" DLL_IMPORT_EXPORT void MDXX_add_function_variable(mdxx::variable_map * variables, const char * variable, mdxx::gen_func func);
extern "C" DLL_IMPORT_EXPORT void MDXX_add_string_variable(mdxx::variable_map * variables, const char * variable, const char * value);
extern "C" DLL_IMPORT_EXPORT mdxx::Expansion_Base * MDXX_get_variable(mdxx::variable_map * variables, const char * variable);
extern "C" DLL_IMPORT_EXPORT bool MDXX_check_if_var_exists(mdxx::variable_map * variables, const char * variable);
extern "C" DLL_IMPORT_EXPORT mdxx::variable_map * MDXX_get_variable_map(mdxx::Plugin_Loader * p, void * id);
extern "C" DLL_IMPORT_EXPORT void MDXX_free_variable_map(mdxx::Plugin_Loader * p, void * id);

#endif
