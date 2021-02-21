#ifndef MDXX_VARIABLE_MAP_H
#define MDXX_VARIABLE_MAP_H
#include "expansion.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace mdxx {

typedef std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> variable_map;

extern "C" void MDXX_add_variable(variable_map * variables, const char * variable, std::unique_ptr<mdxx::Expansion_Base>&& value);
extern "C" void MDXX_add_variable(variable_map * variables, const char * variable, mdxx::gen_func func);
extern "C" void MDXX_add_variable(variable_map * variables, const char * variable, const char * value);

}

#endif // !MDXX_VARIABLE_MAP_H
