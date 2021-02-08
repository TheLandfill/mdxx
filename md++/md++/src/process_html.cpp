#include "process_html.h"

namespace mdxx {

void process_html(HTML_Manager& html, const char * line_ptr, size_t num_lines, bool& in_pre_section) {
	std::string line(line_ptr);
	if (in_pre_section) {
		std::string blank_line = "";
		for (size_t i = 1; i < num_lines; i++) {
			html.add_pre(blank_line);
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

}
