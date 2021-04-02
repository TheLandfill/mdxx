#include "dll_info.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "thread_safe_print.h"
#include "mdxx_get.h"

using namespace mdxx;

extern "C" {

char * MDXX_html_push(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		MDXX_thread_safe_print(stderr, "push needs an HTML_Manager* as its argument.\n");
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	MDXX_get<HTML_Manager*>(args[0])->push();
	return nullptr;
}

char * MDXX_html_pop(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	if (argc < 1) {
		MDXX_thread_safe_print(stderr, "pop needs an HTML_Manager* as its argument.\n");
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	MDXX_get<HTML_Manager*>(args[0])->pop();
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
