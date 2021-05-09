#ifndef MDXX_VARIABLE_PARSER_H
#define MDXX_VARIABLE_PARSER_H
#include "mdxx_token.h"
#include "mdxx_manager.h"
#include <vector>

namespace mdxx {

void parse_line_for_variables(MDXX_Manager *  md, const char * line, std::vector<MDXX_Token>& output);

}

#endif
