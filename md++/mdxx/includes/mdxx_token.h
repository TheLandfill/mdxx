#ifndef MDXX_MDXX_TOKEN_H
#define MDXX_MDXX_TOKEN_H
#include <string_view>

namespace mdxx {

struct MDXX_Token {
	std::string_view str;
	bool is_variable;
};

struct MDXX_Variable_Info {
	std::unordered_set<std::string> dependencies;
	size_t last_line_modified = 0;
};

}

#endif
