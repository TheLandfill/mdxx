#include "variable_parser.h"
#include <iostream>

void print_parts_vec(const std::vector<mdxx::MDXX_Token>& parts) {
	for (auto & part : parts) {
		std::cout << "`" << part.str << "` `" << part.is_variable << "`\n";
	}
	std::cout << "\n";
}

int main() {
	std::vector<mdxx::MDXX_Token> parts;
	parts.reserve(128);
	std::vector<std::string> strings {
		"This is {{a}}{{test}} {{test}} {{abcd}} {{Let's go (mdxx) baseball}}",
		"{{lol}} {{abc def (efg) test}}This is {{a}}{{test}} {{test}} {{abcd}} {{Let's go (mdxx) baseball}}",
		"{{lol}} {{abc def (efg) test}}This is {{a}}{{test}} {{test}} {{abcd}} {{Let's go (mdxx) baseball}} test",
		"{{lol}} {{abc def (efg) test}}This is {{a}}{{test}} {{test}} {{abcd}} {{Let's{ go (mdxx) baseball}}",
		"{{{lol}} {{}abc def (efg) test}}This is {{a}}{{test}} {{test}} {{abcd}} {{Let's{ go (mdxx) baseball}}"
	};
	for (const auto& str : strings) {
		std::cout << "`" << str << "`\n";
		mdxx::parse_line_for_variables(nullptr, str.c_str(), parts);
		print_parts_vec(parts);
	}
	return 0;
}
