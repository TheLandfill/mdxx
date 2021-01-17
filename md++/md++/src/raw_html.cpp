#include "raw_html.h"
#include "html_manager.h"

namespace mdxx {

Raw_HTML::Raw_HTML(const std::string name) : Context(name) {}

void Raw_HTML::open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) {
	(void)html;
	(void)args;
	(void)mdxx;
}

void Raw_HTML::process(HTML_Manager& html, Line_Data& ls) {
	if (in_pre_section) {
		std::string empty_line{""};
		for (size_t i = 0; i < ls.num_lines; i++) {
			html.add_pre(empty_line);
		}
		html.add_pre(ls.line);
	} else {
		html.add(ls.line);
	}
	if (ls.line.find("<pre>") != std::string::npos) {
		in_pre_section = true;
	}
	if (ls.line.find("</pre>") != std::string::npos) {
		in_pre_section = false;
	}
}

void Raw_HTML::close(HTML_Manager& html) {
	(void)html;
}

Raw_HTML::~Raw_HTML() {}

}
