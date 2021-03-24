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
#include "sanitize_user_input.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <filesystem>
#include <locale>
#undef min
#undef max

struct Heading {
	std::string heading_text;
	std::string heading_link;
	char size;
};

struct Headings_Holder {
	std::vector<Heading> headings;
	std::string headings_object_id;
};

template<>
const char * mdxx::Expansion<Headings_Holder*>::to_string() {
	std::stringstream strstr;
	strstr << "<Headings_Holder object @ " << this->get_data() << ">";
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

// author specific
std::string percent_encode_name(const std::string& str) {
	char to_hex[] = "0123456789ABCDEF";
	std::string output;
	output.reserve(str.length() * 3);
	static std::locale loc{"C"};
	for (char c : str) {
		bool is_dash = (c == '-');
		bool is_underscore = (c == '_');
		bool is_dot = (c == '.');
		bool is_tilde = (c == '~');
		bool is_alpha_numeric = isalnum(c, loc);
		bool use_normal = is_dash
			|| is_underscore
			|| is_dot
			|| is_tilde
			|| is_alpha_numeric;
		if (use_normal) {
			output += c;
		} else {
			output += "%";
			output += to_hex[(c >> 4) & 0b1111];
			output += to_hex[c & 0b1111];
		}
	}
	return output;
}

void remove_non_word_characters(std::string& str) {
	size_t index = 0;
	static std::locale loc{"en_US.UTF-8"};
	for (char c : str) {
		bool is_asterisk = (c == '*');
		bool is_dash = (c == '-');
		bool is_alpha_numeric = isalnum(c, loc);
		str[index] = c;
		index += is_asterisk || is_dash || is_alpha_numeric;
	}
	str.erase(index);
}

std::string string_urlify(const std::string& str) {
	std::string output = str;
	mdxx::remove_all_html_chars(output);
	output = url_to_lower(mdxx::join(mdxx::split(output), "-"));
	remove_non_word_characters(output);
	return output;
}

char * get_author_description(mdxx::Expansion_Base** args, size_t argc) {
	if (argc < 3) {
		std::cerr << "ERROR: Need to provide the path to the author descriptions, the author's name, and the total number of allowable characters.\n";
	}
	std::string author_directory = *static_cast<const char **>(args[0]->get_data());
	std::string author_name = string_urlify(*static_cast<const char **>(args[1]->get_data()));
	size_t description_length = strtoul(*static_cast<const char **>(args[2]->get_data()), nullptr, 10);
	const static std::string description = "description.txt";
	std::string file = author_directory + author_name;
	file += std::filesystem::path::preferred_separator;
	file += description;
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
	if (argc < 3) {
		std::cerr <<
"ERROR: heading_to_section requires (headings-do-not-touch), the heading text,\n"
"and the heading size (any number from 1 - 6) as the arguments\n";
		exit(EXIT_FAILURE);
	}
	Headings_Holder * hh = *static_cast<Headings_Holder**>(args[0]->get_data());
	const char * heading_size_str = *static_cast<const char**>(args[1]->get_data());
	std::string heading_text;
	heading_text.reserve(1024);
	for (size_t i = 2; i < argc; i++) {
		heading_text += *static_cast<const char**>(args[i]->get_data());
	}
	std::string heading_link = string_urlify(heading_text);
	char heading_size = std::max(std::min((unsigned long)6, strtoul(heading_size_str, nullptr, 10)), (unsigned long)1);
	hh->headings.push_back({heading_text, heading_link, heading_size});
	std::string out;
	out.reserve(20 + heading_link.length() + heading_text.length());
	out += "<h";
	out += heading_size + '0';
	out += " id=\"";
	out += heading_link;
	out += "\">";
	out += heading_text;
	out += "</h";
	out += heading_size + '0';
	out += ">";
	return mdxx::c_string_copy(out);
}

char * sidenav(mdxx::Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr << "ERROR: sidenav requires (headings-do-not-touch) and (html) as the arguments.\n";
		exit(EXIT_FAILURE);
	}
	Headings_Holder * hh = *static_cast<Headings_Holder**>(args[0]->get_data());
	mdxx::HTML_Manager * html = *static_cast<mdxx::HTML_Manager**>(args[1]->get_data());
	MDXX_html_add(html, "<nav id=\"sidenav\">");
	std::string dummy = "";
	for (auto& heading : hh->headings) {
		MDXX_html_add_no_nl(html, "<a href=\"#");
		MDXX_html_write(html, heading.heading_link.c_str());
		MDXX_html_write(html, "\" style=\"padding-left: ");
		MDXX_html_write(html, std::to_string(heading.size * 12).c_str());
		MDXX_html_write(html, "px;\">");
		MDXX_html_write(html, heading.heading_text.c_str());
		MDXX_html_add_pre(html, "</a>");
	}
	MDXX_html_add(html, "</nav>");
	MDXX_html_add(html, "<div id=\"sidenav-activator\"></div>");
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
	MDXX_add_string_variable_to_context(mdxx, "default", "h", "{{heading-to-section (headings-do-not-touch) [1:]}}");
	MDXX_add_function_variable_to_context(mdxx, "template", "prefix-suffix-loop-func", prefix_suffix_loop_func);
	MDXX_add_function_variable_to_context(mdxx, "default", "heading-to-section", heading_to_section);
	MDXX_add_function_variable_to_context(mdxx, "default", "urlify", urlify);
	MDXX_add_function_variable_to_context(mdxx, "template", "author-description", get_author_description);
	MDXX_add_function_variable_to_context(mdxx, "template", "sidenav-func", sidenav);
	MDXX_add_string_variable_to_context(mdxx, "template", "sidenav", "{{sidenav-func (headings-do-not-touch) (html)}}");
	MDXX_add_general_variable_to_context(mdxx, "default", "headings-do-not-touch", new mdxx::Expansion<Headings_Holder*>(new Headings_Holder{}));
}

extern "C" DLL_IMPORT_EXPORT void print_compilation_info() {
	std::cout << "tuacm:\t\t" << MDXX_COMPILATION_INFO << ".\n";
}
