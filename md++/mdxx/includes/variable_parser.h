#ifndef MDXX_VARIABLE_PARSER_H
#define MDXX_VARIABLE_PARSER_H
#include "mdxx_manager.h"
#include <string_view>
#include <vector>

namespace mdxx {

struct MDXX_Token {
	std::string_view str;
	bool is_variable;
};

void parse_line_for_variables(MDXX_Manager *  md, const char * line, std::vector<MDXX_Token>& output);

}

#endif
