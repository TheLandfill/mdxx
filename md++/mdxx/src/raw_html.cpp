#include "raw_html.h"
#include "html_manager.h"
#include "variable_map.h"

namespace mdxx {

Raw_HTML::Raw_HTML(const char * n) : name(n) {}

void Raw_HTML::open(HTML_Manager& html, const char * args) {
	(void)html;
	(void)args;
}

void Raw_HTML::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	std::string line(line_ptr);
	if (in_pre_section) {
		std::string empty_line{""};
		for (size_t i = 0; i < num_lines; i++) {
			html.add_pre(empty_line);
		}
		html.add_pre(line);
	} else {
		html.add(line);
	}
	if (line.find("<pre>") != std::string::npos) {
		in_pre_section = true;
	}
	if (line.find("</pre>") != std::string::npos) {
		in_pre_section = false;
	}
}

void Raw_HTML::close(HTML_Manager& html) {
	(void)html;
}

Raw_HTML::~Raw_HTML() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Raw_HTML)

}
