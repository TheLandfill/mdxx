#include "context.h"
#include <memory>

namespace mdxx {

Context::Context(std::string n, variable_map& v) :
	name(n),
	variables(std::move(v))
{}

Context::Context(std::string n) :
	name(n),
	variables()
{}

variable_map& Context::get_variables() {
	return variables;
}

template<>
void Context::add_variable<gen_func>(std::string variable_name, gen_func variable_value) {
	variables[variable_name] = std::make_unique<Expansion<gen_func>>(variable_value, variable_name);
}

}
