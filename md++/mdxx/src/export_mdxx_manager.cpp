#include "mdxx_manager.h"
#include "context.h"
#include <memory>
#include <cstring>
#include <iostream>

extern "C" {

void MDXX_add_new_context(mdxx::MDXX_Manager * mdxx, const char * name, mdxx::Context * context) {
	std::unique_ptr<mdxx::Context> test(context);
	mdxx->add_raw_context(name, test);
}

char * MDXX_print_current_line(mdxx::MDXX_Manager * mdxx) {
	std::string temp = mdxx->print_line();
	char * str = new char[temp.length() + 1];
	strncpy(str, temp.c_str(), temp.length() + 1);
	return str;
}

void MDXX_print_current_line_and_exit(mdxx::MDXX_Manager * md) {
	std::cerr << "\nLine that caused the problem:\n";
	char * cur_line = MDXX_print_current_line(md);
	std::cerr << cur_line;
	delete[] cur_line;
	exit(EXIT_FAILURE);
}

}
