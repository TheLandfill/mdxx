#include "context.h"
#include <memory>

namespace mdxx {

Context::Context(const char * n) :
	name(n),
	variables()
{}

variable_map& Context::get_variables() {
	return variables;
}

template<>
void Context::add_variable(const char * variable_name, const char * variable_value) {
	variables[std::string(variable_name)] = std::make_unique<Expansion<std::string>>(variable_value);
}

template<>
void Context::add_variable<gen_func>(const char * variable_name, gen_func variable_value) {
	variables[std::string(variable_name)] = std::make_unique<Expansion<gen_func>>(variable_value, variable_name);
}

Context::~Context() {}

}
