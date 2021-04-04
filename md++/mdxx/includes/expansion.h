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

#ifndef MDXX_EXPANSION_H
#define MDXX_EXPANSION_H
#include "dll_info.h"
#include <string>
#include <sstream>
#include <vector>
#include <memory>

namespace mdxx {

#define MDXX_EXP(X, Y) new mdxx::Expansion<X>(Y)

struct Expansion_Base {
	virtual void * get_data() = 0;
	virtual const char * to_string() = 0;
	virtual Expansion_Base* make_deep_copy() = 0;
	virtual const char * get_type() = 0;
};

class MDXX_Manager;

typedef char * (*gen_func)(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

template<typename T>
struct Expansion : Expansion_Base {
	T data;
	std::string to_str;
	std::string type;
	Expansion(T d) : data(d), type(typeid(T).name()) {}
	void * get_data() override {
		return &data;
	}
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	const char * get_type() override;
	~Expansion() {}
};

template<>
struct Expansion<std::string> : Expansion_Base {
	std::string data;
	std::string type = typeid(std::string).name();
	Expansion(std::string d);
	void * get_data() override;
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	const char * get_type() override;
};

template <typename T>
struct Expansion<T *> : Expansion_Base {
	T * data;
	std::string to_str;
	std::string type;
	Expansion(T * d) : data(d), type(typeid(T*).name()) {}
	void * get_data() override {
		return &data;
	}
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	const char * get_type() override;
	~Expansion() {}
};

template<>
struct Expansion<gen_func> : Expansion_Base {
	gen_func func;
	std::string name;
	std::string type = typeid(gen_func).name();
	std::string full_name;
	Expansion(gen_func function, std::string name);
	void * get_data() override;
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	const char * get_type() override;
};

template<typename T>
const char * Expansion<T>::to_string() {
	to_str = std::string(data);
	return to_str.c_str();
}

template<typename T>
const char * Expansion<T>::get_type() {
	return type.c_str();
}

template<typename T>
const char * Expansion<T*>::to_string() {
	std::stringstream strstr;
	strstr << std::string(*data) << " (Pointer) @ " << data;
	to_str = strstr.str();
	return to_str.c_str();
}

template<typename T>
const char * Expansion<T*>::get_type() {
	return type.c_str();
}

template<>
Expansion<char *>::Expansion(char * data);

template<>
const char * Expansion<char *>::to_string();

template<>
const char * Expansion<const char *>::to_string();

template<typename T>
Expansion_Base* Expansion<T>::make_deep_copy() {
	return new Expansion<T>(data);
}

template<typename T>
Expansion_Base* Expansion<T *>::make_deep_copy() {
	return new Expansion<T*>(data);
}

}

#endif
