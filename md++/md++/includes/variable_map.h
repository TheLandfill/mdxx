#ifndef MDXX_VARIABLE_MAP_H
#define MDXX_VARIABLE_MAP_H
#include "expansion.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace mdxx {

typedef std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> variable_map;

extern "C" void MDXX_add_general_variable(variable_map * variables, const char * variable, std::unique_ptr<Expansion_Base>&& value);
extern "C" void MDXX_add_function_variable(variable_map * variables, const char * variable, gen_func func);
extern "C" void MDXX_add_string_variable(variable_map * variables, const char * variable, const char * value);
extern "C" Expansion_Base * MDXX_get_variable(variable_map * variables, const char * variable);
extern "C" bool MDXX_check_if_var_exists(variable_map * variables, const char * variable);

}

#endif // !MDXX_VARIABLE_MAP_H
