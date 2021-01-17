#include "template_context.h"
#include "expansion.h"
#include "html_manager.h"
#include "process_html.h"

namespace mdxx {

Template_Context::Template_Context(const std::string name) : Context(name) {}

void Template_Context::open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) {
	(void)html;
	(void)args;
	(void)mdxx;
	return;
}

void Template_Context::process(HTML_Manager& html, Line_Data& line_data) {
	process_html(html, line_data, in_pre_section);
}

void Template_Context::close(HTML_Manager& html) {
	(void)html;
	return;
}

Template_Context::~Template_Context() {}

}
