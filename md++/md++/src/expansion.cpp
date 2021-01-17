#include "expansion.h"
#include <memory>

namespace mdxx {

Expansion_Base::~Expansion_Base() {}

void * Expansion_Base::get_data() {
	return nullptr;
}

std::string Expansion_Base::to_string() {
	return R"lol(This is an empty expansion base that only exists because I
	need a default constructor to fill up the empty parts of the table.)lol";
}

std::unique_ptr<Expansion_Base> Expansion_Base::make_deep_copy() {
	return nullptr;
}

std::string Expansion<gen_func>::to_string() {
	return name + " (function)";
}

Expansion<gen_func>::Expansion(gen_func function, std::string n) : func(function), name(n) {}

void * Expansion<gen_func>::get_data() {
	return &name;
}

template<>
std::string Expansion<const char *>::to_string() {
	return std::string((const char *)data);
}

template<>
std::string Expansion<char *>::to_string() {
	return std::string((char *)data);
}

std::unique_ptr<Expansion_Base> Expansion<gen_func>::make_deep_copy() {
	return std::make_unique<Expansion<gen_func>>( func, name );
}

}
