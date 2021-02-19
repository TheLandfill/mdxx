#include "content_manager.h"
#include <memory>
#include <sstream>
#include <vector>
#include <iostream>

namespace mdxx {

Content_Manager::Content_Manager(HTML_Manager& h, MDXX_Manager& m) : html(h), mdxx(m) {
	mdxx.set_context({ "default" });
}

void Content_Manager::process_content() {
	while (true) {
		std::string line = mdxx.find_and_return_next_content_line();
		if (mdxx.at_end_of_file()) {
			break;
		}
		mdxx.handle_context(html);
	}
	html.check_and_close_paragraph();
}

MDXX_Manager& Content_Manager::get_mdxx() {
	return mdxx;
}

template<>
const char * Expansion<Content_Manager*>::to_string() {
	std::stringstream output;
	output << "<Content_Manager @ " << data << ">";
	data->content_object_id = output.str();
	return data->content_object_id.c_str();
}

}
