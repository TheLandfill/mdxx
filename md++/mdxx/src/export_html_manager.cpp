#include "dll_info.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "mdxx_get.h"
#include <iostream>

using namespace mdxx;

extern "C" {

char * MDXX_html_push(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "push needs an HTML_Manager* as its argument." << std::endl;
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	(*static_cast<HTML_Manager**>(args[0]->get_data()))->push();
	return nullptr;
}

char * MDXX_html_pop(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	if (argc < 1) {
		std::cerr << "pop needs an HTML_Manager* as its argument." << std::endl;
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	(*static_cast<HTML_Manager**>(args[0]->get_data()))->pop();
	return nullptr;
}

void MDXX_html_add(HTML_Manager* html, const char * line) {
	html->add(line);
}

void MDXX_html_add_pre(HTML_Manager* html, const char * line) {
	html->add_pre(line);
}

void MDXX_html_add_no_nl(HTML_Manager* html, const char * line) {
	html->add_no_nl(line);
}

void MDXX_html_write(HTML_Manager* html, const char * line) {
	html->write(line);
}

}
