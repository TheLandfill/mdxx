#include "expansion.h"
#include <cstring>
#include <memory>

namespace mdxx {

Expansion_Base::~Expansion_Base() {}

const char * Expansion<gen_func>::to_string() {
	return full_name.c_str();
}

Expansion<std::string>::Expansion(std::string d) : data(d) {}

void * Expansion<std::string>::get_data() {
	return &data;
}

const char * Expansion<std::string>::to_string() {
	return data.c_str();
}

Expansion_Base* Expansion<std::string>::make_deep_copy() {
	char * copy = new char[data.length() + 1];
	strcpy(copy, data.c_str());
	return new Expansion<char *>(copy);
}

Expansion<gen_func>::Expansion(gen_func function, std::string n) : func(function),
	name(n),
	full_name(name + " (function)")
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
	std::stringstream strstr;
	strstr << data << " (char *)";
	to_str = strstr.str();
	return to_str.c_str();
}

template<>
Expansion<char *>::~Expansion() {
	delete[] data;
}

}
