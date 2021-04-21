// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
		error_message += "Someone has passed in an argument of type\n\t`";
		error_message += exp->get_type();
		error_message += "`\nwhen the expected type was\n\t`";
		error_message += typeid(T).name();
		error_message += "`\nLikely causes include a user redefining a builtin variable, a user redefining a\n"
			"plugin variable, or errors in a plugin.";
		throw std::runtime_error(error_message);
	}
}

#endif
