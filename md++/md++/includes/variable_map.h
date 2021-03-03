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
