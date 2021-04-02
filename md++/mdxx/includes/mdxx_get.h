#ifndef MDXX_GET_H
#define MDXX_GET_H
#include "expansion.h"
#include <stdexcept>

#define MDXX_GET(T, Y) MDXX_get<T>(Y)
template<typename T>
T MDXX_get(mdxx::Expansion_Base * exp) {
	if (std::string(exp->get_type()) == typeid(T).name()) {
		return *static_cast<T*>(exp->get_data());
	} else {
		std::string error_message;
		error_message.reserve(2048);
		error_message += "ERROR: Someone has passed in an argument of type\n\t`";
		error_message += exp->get_type();
		error_message += "`\nwhen the expected type was\n\t`";
		error_message += typeid(T).name();
		error_message += "`\nLikely causes include a user redefining a builtin or plugin variable or errors in a plugin.\n";
		throw std::runtime_error(error_message);
	}
}

#endif
