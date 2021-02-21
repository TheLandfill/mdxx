#include "template_context.h"
#include "expansion.h"
#include "html_manager.h"
#include "process_html.h"

namespace mdxx {

Template_Context::Template_Context(const char * n) : name(n) {}

void Template_Context::open(HTML_Manager& html, const char * args) {
	(void)html;
	(void)args;
	return;
}

void Template_Context::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	process_html(html, line_ptr, num_lines, in_pre_section);
}

void Template_Context::close(HTML_Manager& html) {
	(void)html;
	return;
}

Template_Context::~Template_Context() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Template_Context)

}
