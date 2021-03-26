#include "content_manager.h"
#include "sanitize_user_input.h"
#include "clear_line.h"
#include <memory>
#include <sstream>
#include <vector>
#include <iostream>

namespace mdxx {

Content_Manager::Content_Manager(HTML_Manager& h, MDXX_Manager& m, std::string in) : html(h), mdxx(m), infile(in) {
	mdxx.set_context({ "default" });
}

void Content_Manager::process_content() {
	while (true) {
		mdxx.find_next_content_line();
		mdxx.replace_angle_brackets_in_line();
		mdxx.sanitize_ampersands();
		if (mdxx.at_end_of_file()) {
			break;
		}
		mdxx.handle_context(html);
	}
	html.check_and_close_paragraph();
	if (mdxx.had_error()) {
		std::cerr << MDXX_CLEAR_LINE << "\nERROR DETECTED in\n\t" << infile << "\nOutput will be deleted.\n"
		"--------------------------------------------------------------------------------" << std::endl;
		html.delete_outfile();
	}
}

MDXX_Manager& Content_Manager::get_mdxx() {
	return mdxx;
}

std::string Content_Manager::get_infile() {
	return infile;
}

template<>
const char * Expansion<Content_Manager*>::to_string() {
	std::stringstream output;
	output << "<Content_Manager @ " << data << ">";
	data->content_object_id = output.str();
	return data->content_object_id.c_str();
}

}
