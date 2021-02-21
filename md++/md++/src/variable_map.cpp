#include "variable_map.h"

namespace mdxx {

void MDXX_add_general_variable(variable_map * variables, const char * variable, std::unique_ptr<Expansion_Base>&& value) {
	(*variables)[std::string(variable)] = std::move(value);
}

void MDXX_add_function_variable(variable_map * variables, const char * variable, gen_func func) {
	(*variables)[std::string(variable)] = std::make_unique<Expansion<gen_func>>(func, std::string(variable));
}

void MDXX_add_string_variable(variable_map * variables, const char * variable, const char * value) {
	(*variables)[std::string(variable)] = std::make_unique<Expansion<std::string>>(value);
}

Expansion_Base * MDXX_get_variable(variable_map * variables, const char * variable) {
	return variables->at(std::string(variable)).get();
}

bool MDXX_check_if_var_exists(variable_map * variables, const char * variable) {
	return variables->count(std::string(variable)) > 0;
}

}