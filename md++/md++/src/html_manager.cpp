#include "html_manager.h"
#include "default.h"
#include "re2/re2.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>

namespace mdxx {

HTML_Manager::HTML_Manager(std::ofstream& o) : out(o) {
	tab_level.reserve(32);
}

HTML_Manager::HTML_Manager(std::ofstream&& o) : out(o) {
	tab_level.reserve(32);
}

void HTML_Manager::add(std::string& line) {
	write(tab_level);
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_pre(std::string& line) {
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_no_nl(std::string& line) {
	write(tab_level);
	write_empty(line);
}

void HTML_Manager::add(const char * line) {
	write(tab_level);
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_pre(const char * line) {
	write_empty(line);
	write("\n");
}

void HTML_Manager::add_no_nl(const char * line) {
	write(tab_level);
	write_empty(line);
}

void HTML_Manager::write(std::string str) {
	out << str << std::flush;
}

void HTML_Manager::write_empty(std::string str) {
	remove_empty(str);
	write(str);
}

void HTML_Manager::remove_empty(std::string& str) {
	static const re2::RE2 empty_regex(empty_str);
	re2::RE2::GlobalReplace(&str, empty_regex, "");
}

void HTML_Manager::check_and_close_paragraph() {
	if (need_to_close_paragraph) {
		pop();
		write(tab_level);
		write("</p>\n");
		need_to_close_paragraph = false;
	}
}

void HTML_Manager::push() {
	tab_level += "\t";
}

void HTML_Manager::pop() {
	if (tab_level.length() > 0) {
		tab_level.pop_back();
	} else {
		std::cerr << "WARNING: You popped the tab_level when it was at zero!\n";
	}
}

void HTML_Manager::open_paragraph() {
	need_to_close_paragraph = true;
}

template<>
const char * Expansion<HTML_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<HTML_Manager object @ " << this->get_data() << ">";
	data->html_object_id = strstr.str();
	return data->html_object_id.c_str();
}

template<>
Expansion_Base* Expansion<HTML_Manager>::make_deep_copy() {
	throw std::logic_error("ERROR: HTML_Manager should not be copied!");
}

}
