#include "expansion.h"
#include <memory>

namespace mdxx {

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
	return name;
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
	return MAKE_GEN_FUNC_EXPANSION(func);
}

}
