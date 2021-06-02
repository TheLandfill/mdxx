#ifndef MDXX_MDXX_TOKEN_H
#define MDXX_MDXX_TOKEN_H
#include <string_view>

namespace mdxx {

struct MDXX_Token {
	std::string_view str;
	bool is_variable;
};

}

#endif
