#define MDXX_INTERFACE_DLL
#include "dll_info.h"
#include "html_manager.h"
#include <iostream>

using namespace mdxx;

extern "C" {

char * MDXX_html_push(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "push needs an Expansion<HTML_Manager**> as its argument." << std::endl;
		exit(EXIT_FAILURE);
	}
	(*static_cast<HTML_Manager**>(args[0]->get_data()))->push();
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

char * MDXX_html_pop(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "pop needs an Expansion<HTML_Manager**> as its argument." << std::endl;
		exit(EXIT_FAILURE);
	}
	(*static_cast<HTML_Manager**>(args[0]->get_data()))->pop();
	return nullptr;
}

}
