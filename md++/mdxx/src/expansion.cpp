#include "expansion.h"
#include "c_string_copy.h"
#include <memory>

namespace mdxx {

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
	return new Expansion<char *>(c_string_copy(data));
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
	strstr << data;
	to_str = strstr.str();
	return to_str.c_str();
}

template<>
Expansion<char *>::~Expansion() {
	delete[] data;
}

}
