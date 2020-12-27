#include <fstream>
#include <iostream>
#include <memory>
#include "mdxx_manager.h"
#include "html_manager.h"
#include "content_manager.h"
#include "template_manager.h"

void usage_message(char * program_name);

int main(int argc, char ** argv) {
	using namespace mdxx;
	if (argc < 3) {
		usage_message(argv[0]);
		return 1;
	}
	std::string template_path(argv[1]);
	std::string infile(argv[2]);
	std::ifstream in{infile};
	MDXX_Manager mdxx = MDXX_Manager(in);
	std::string outfile = infile.substr(0, infile.find_last_of(".")) + ".html";
	std::ofstream out{outfile};
	HTML_Manager html{out};
	std::shared_ptr<Content_Manager> content = std::make_shared<Content_Manager>(html, mdxx);
	std::string template_file = template_path + mdxx.next_line_no_nl();
	Template_Manager template_reader(html, content, template_file);
	template_reader.process_template();
	return 0;
}

void usage_message(char * program_name) {
	std::cerr << program_name << " template_location mdxx_file\n";
}
