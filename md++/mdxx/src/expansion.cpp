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

#include "expansion.h"
#include "c_string_copy.h"
#include "mdxx_ansi.h"
#include <memory>

namespace mdxx {

template<>
Expansion<char *>::Expansion(char * d) : data(d), type(typeid(const char *).name()) {}

const char * Expansion<gen_func>::to_string() {
	return full_name.c_str();
}

const char * Expansion<gen_func>::get_type() {
	return type.c_str();
}

Expansion<std::string>::Expansion(std::string d) : data(d) {}

void * Expansion<std::string>::get_data() {
	return &data;
}

const char * Expansion<std::string>::to_string() {
	return data.c_str();
}

const char * Expansion<std::string>::get_type() {
	return type.c_str();
}

Expansion_Base* Expansion<std::string>::make_deep_copy() {
	return new Expansion<char *>(c_string_copy(data));
}

Expansion<gen_func>::Expansion(gen_func function, std::string n) : func(function),
	name(n),
	full_name(((name + MDXX_FUNC_COLOR) + " (function)") + MDXX_RESET)
{}

Expansion_Base* Expansion<gen_func>::make_deep_copy() {
	return new Expansion<gen_func>( func, name );
}

void * Expansion<gen_func>::get_data() {
	return &name;
}

template<>
const char * Expansion<const char *>::to_string() {
	return data;
}

template<>
const char * Expansion<char *>::to_string() {
	return data;
}

template<>
Expansion<char *>::~Expansion() {
	delete[] data;
}

}
