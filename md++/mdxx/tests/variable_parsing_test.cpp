#include "variable_parser.h"
#include <iostream>

int main() {
	std::vector<mdxx::MDXX_Token> parts;
	parts.reserve(128);
	mdxx::parse_line_for_variables(nullptr, "This is {{a}}{{test}} {{test}} {{abcd}} {{Let's go (mdxx) baseball}}", parts);
	for (auto & part : parts) {
		std::cout << "`" << part.str << "` `" << part.is_variable << "`\n";
	}
	mdxx::parse_line_for_variables(nullptr, "{{lol}} {{abc def (efg) test}}This is {{a}}{{test}} {{test}} {{abcd}} {{Let's go (mdxx) baseball}}", parts);
	for (auto & part : parts) {
		std::cout << "`" << part.str << "` `" << part.is_variable << "`\n";
	}
	return 0;
}
