#define MDXX_INTERFACE_DLL
#include "dll_info.h"
#include "variable_map.h"
#include "plugin_loader.h"

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
	(*variables)[std::string(variable)] = std::make_unique<Expansion<std::string>>(value);
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
