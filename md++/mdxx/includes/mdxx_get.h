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
#include <cstring>
#include <stdexcept>

namespace mdxx {

std::string MDXX_get_print_error_message(const char * received, const char * actual);

}

template<typename T>
bool MDXX_is_type(mdxx::Expansion_Base * exp) {
	return !strcmp(exp->get_type(), typeid(T).name());
}

#define MDXX_GET(T, Y) MDXX_get<T>(Y)
template<typename T>
T MDXX_get(mdxx::Expansion_Base * exp) {
	if (MDXX_is_type<T>(exp)) {
		return *static_cast<T*>(exp->get_data());
	} else {
		std::string error_message = mdxx::MDXX_get_print_error_message(exp->get_type(), typeid(T).name());
		throw std::runtime_error(error_message);
	}
}

#endif
