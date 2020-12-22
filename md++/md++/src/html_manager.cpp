#include "html_manager.h"
#include "default.h"
#include "re2/re2.h"
#include <memory>
#include <vector>
#include <sstream>

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

void HTML_Manager::write(std::string str) {
	out << str;
}

void HTML_Manager::write_empty(std::string str) {
	remove_empty(str);
	out << str;
}

void HTML_Manager::remove_empty(std::string& str) {
	static const re2::RE2 empty_regex(empty_str);
	re2::RE2::Replace(&str, empty_regex, "");
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
	tab_level += '\t';
}

void HTML_Manager::pop() {
	tab_level.pop_back();
}

std::string HTML_Manager::push(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	static_cast<HTML_Manager*>(args.at(0)->get_data())->push();
	return "";
}

std::string HTML_Manager::pop(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	static_cast<HTML_Manager*>(args.at(0)->get_data())->pop();
	return "";
}

template<>
std::string Expansion<HTML_Manager>::to_string() {
	std::stringstream strstr;
	strstr << "<HTML_Manager object @ " << (void*)this << ">";
	return strstr.str();
}

}