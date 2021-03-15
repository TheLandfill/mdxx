#define MDXX_EXTERNAL_CONTEXT
#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include "plugin_loader.h"
#include "variable_map.h"
#include "split.h"
#include "join.h"
#include "c_string_copy.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

struct Headings_Holder {
	std::vector<std::string> headings;
	std::string headings_object_id;
};

template<>
const char * mdxx::Expansion<Headings_Holder*>::to_string() {
	std::stringstream strstr;
	strstr << "<HTML_Manager object @ " << this->get_data() << ">";
	data->headings_object_id = strstr.str();
	return data->headings_object_id.c_str();
}


std::string url_to_lower(std::string str) {
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		[](unsigned char c){
			return std::tolower(c);
		}
	);
	return str;
}

std::string string_urlify(const std::string& str) {
	return url_to_lower(mdxx::join(mdxx::split(str), "-"));
}

char * get_author_description(mdxx::Expansion_Base** args, size_t argc) {
	if (argc < 3) {
		std::cerr << "ERROR: Need to provide the path to the author descriptions, the author's name, and the total number of allowable characters.\n";
	}
	std::string author_directory = *static_cast<const char **>(args[0]->get_data());
	std::string author_name = string_urlify(*static_cast<const char **>(args[1]->get_data()));
	size_t description_length = strtoul(*static_cast<const char **>(args[2]->get_data()), nullptr, 10);
	const static std::string description = "/description.txt";
	std::string file = author_directory + author_name + description;
	std::ifstream in{file};
	if (!in.is_open()) {
		std::cerr << "ERROR: Could not read the file `" << file << "`.\n";
		exit(EXIT_FAILURE);
	}
	in.seekg(0, in.end);
	size_t file_length = in.tellg();
	in.seekg(0, in.beg);
	if (file_length > description_length) {
		std::cerr << "WARNING: File " << author_directory << author_name << description << " has more characters than the template allows.\n"
		<< "mdxx will stop reading after " << description_length << " characters.\n";
	}
	size_t num_to_read = std::min(description_length, file_length);
	char * output = new char[num_to_read + 1];
	in.read(output, num_to_read);
	output[in.gcount()] = '\0';
	return output;
}

char * urlify(mdxx::Expansion_Base** args, size_t argc) {
	if (argc == 0) {
		std::cerr << "ERROR: Need to provide at least one argument\n";
		exit(EXIT_FAILURE);
	}
	std::string pre_url = *static_cast<const char **>(args[0]->get_data());
	std::string url_version = string_urlify(pre_url);
	return mdxx::c_string_copy(url_version);
}

char * heading_to_section(mdxx::Expansion_Base** args, size_t argc) {
	(void)args;
	(void)argc;
	std::cout << "Will need to implment " << __func__ << " in " << __FILE__ << ".\n";
	return nullptr;
}

char * sidenav(mdxx::Expansion_Base** args, size_t argc) {
	(void)args;
	(void)argc;
	std::cout << "Will need to implment " << __func__ << " in " << __FILE__ << ".\n";
	return nullptr;
}

char * prefix_suffix_loop_func(mdxx::Expansion_Base** args, size_t argc) {
	if (argc < 3) {
		std::cerr << "ERROR: Must provide the html manager, prefix, and suffix." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string all_lines;
	all_lines.reserve(8192);
	mdxx::HTML_Manager* html = *static_cast<mdxx::HTML_Manager**>(args[0]->get_data());
	const char * prefix = *static_cast<const char**>(args[1]->get_data());
	const char * suffix = *static_cast<const char**>(args[2]->get_data());
	for (size_t i = 3; i < argc; i++) {
		const char * arg = *static_cast<const char**>(args[i]->get_data());
		auto sheets = mdxx::split(arg);
		for (auto& sheet : sheets) {
			MDXX_html_add_no_nl(html, prefix);
			MDXX_html_write(html, sheet.c_str());
			MDXX_html_add_pre(html, suffix);
		}
	}
	return nullptr;
}

extern "C" DLL_IMPORT_EXPORT void import_plugin(mdxx::Plugin_Loader * pl, mdxx::MDXX_Manager * mdxx) {
	(void)pl;
	MDXX_add_string_variable_to_context(mdxx, "template", "css-prefix", "<link rel=\"stylesheet\" href=\"{{css}}", true);
	MDXX_add_string_variable_to_context(mdxx, "template", "css-suffix", ".css\">");
	MDXX_add_string_variable_to_context(mdxx, "template", "js-prefix", "<script src=\"{{js}}", true);
	MDXX_add_string_variable_to_context(mdxx, "template", "js-suffix", "\".js></script>");
	MDXX_add_string_variable_to_context(mdxx, "template", "style", "{{prefix-suffix-loop-func (html) (css-prefix) (css-suffix) (stylesheets)}}");
	MDXX_add_string_variable_to_context(mdxx, "template", "write-scripts", "{{prefix-suffix-loop-func (html) (js-prefix) (js-suffix) (scripts)}}");
	MDXX_add_string_variable_to_context(mdxx, "default", "h", "{{heading-to-section (headings-do-not-touch) [:]}}");
	MDXX_add_function_variable_to_context(mdxx, "template", "prefix-suffix-loop-func", prefix_suffix_loop_func);
	MDXX_add_function_variable_to_context(mdxx, "default", "heading-to-section", heading_to_section);
	MDXX_add_function_variable_to_context(mdxx, "default", "urlify", urlify);
	MDXX_add_function_variable_to_context(mdxx, "template", "author-description", get_author_description);
	MDXX_add_function_variable_to_context(mdxx, "template", "sidenav", sidenav);
	MDXX_add_general_variable_to_context(mdxx, "default", "headings-do-not-touch", new mdxx::Expansion<Headings_Holder*>(new Headings_Holder{}));
}

extern "C" DLL_IMPORT_EXPORT void print_compilation_info() {
	std::cout << "tuacm:\t\t" << MDXX_COMPILATION_INFO << ".\n";
}
