#include "variable_map.h"

void MDXX_add_variable(variable_map * variables, const char * variable, std::unique_ptr<mdxx::Expansion_Base>&& value) {
	(*variables)[std::string(variable)] = std::move(value);
}

void MDXX_add_variable(variable_map * variables, const char * variable, mdxx::gen_func func) {
	(*variables)[std::string(variable)] = std::make_unique<mdxx::Expansion<mdxx::gen_func>>(func, std::string(variable));
}

void MDXX_add_variable(variable_map * variables, const char * variable, const char * value) {
	(*variables)[std::string(variable)] = std::make_unique<mdxx::Expansion<std::string>>(value);
}