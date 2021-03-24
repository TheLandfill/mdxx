#ifndef MDXX_CURLY_BRACES_SUB_H
#define MDXX_CURLY_BRACES_SUB_H
#include "html_manager.h"
#include "mdxx_manager.h"

namespace mdxx {

struct Autosub {
	const char * var;
	const char * value;
	bool need_to_close_paragraph = false;
};

const extern Autosub curly_autosubs[];

void handle_curly_braces(std::string* line, HTML_Manager& html, MDXX_Manager& mdxx);

};

#endif
