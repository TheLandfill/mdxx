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
	MDXX_Manager mdxx = MDXX_Manager(std::ifstream(infile));
	std::string outfile = infile.substr(0, infile.find_last_of(".")) + ".html";
	HTML_Manager html{std::ofstream(outfile)};
	std::shared_ptr<Content_Manager> content = get_content_manager(html, mdxx);
	Template_Manager template_reader(html, content, template_path);
	template_reader.process_template();
	return 0;
}

void usage_message(char * program_name) {
	std::cerr << program_name << "\ttemplate_location\ttumd_file[s]\n";
}
