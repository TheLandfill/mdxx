#include "context.h"

namespace mdxx {

Context::Context(std::string n, variable_map& v) : name(n), variables(std::move(v)) {}

variable_map& Context::get_variables() {
	return variables;
}

}
