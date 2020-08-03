#include "process_html.h"

namespace mdxx {

void process_html(HTML_Manager& html, const Line_Data& line_data, bool& in_pre_section) {
	std::string line = line_data.line;
	if (in_pre_section) {
		std::string blank_line = "";
		for (size_t i = 1; i < line_data.num_lines; i++) {
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
