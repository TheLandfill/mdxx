#include "mdxx_manager.h"

namespace mdxx {

bool Var_ID::operator==(const Var_ID& other) const {
	return variable_name == other.variable_name && context == other.context;
}

}

std::size_t std::hash<mdxx::Var_ID>::operator()(const mdxx::Var_ID& k) const {
	using std::size_t;
	using std::hash;
	using std::string;
	return (
		hash<string>()(k.context)
		^ (hash<string>()(k.variable_name) << 1)
	);
}
