#ifndef MDXX_HTML_MANAGER_H
#define MDXX_HTML_MANAGER_H
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "expansion.h"

namespace mdxx {

class HTML_Manager {
public:
	HTML_Manager(std::ofstream& o);
	HTML_Manager(std::ofstream&& o);
	void add(std::string& line);
	void add_pre(std::string& line);
	void add_no_nl(std::string& line);
	void add(const char * line);
	void add_pre(const char * line);
	void add_no_nl(const char * line);
	void write(const std::string line);
	void write_empty(std::string str);
	void remove_empty(std::string& line);
	void check_and_close_paragraph();
	void push();
	void pop();
	void open_paragraph();
	static std::string push(std::vector<std::unique_ptr<Expansion_Base>>& args);
	static std::string pop(std::vector<std::unique_ptr<Expansion_Base>>& args);
	HTML_Manager(const HTML_Manager& html) = delete;
	HTML_Manager& operator=(const HTML_Manager& html) = delete;
	HTML_Manager(const HTML_Manager&& html) = delete;
	HTML_Manager& operator=(const HTML_Manager&& html) = delete;
private:
	std::string tab_level = "";
	std::ofstream& out;
	bool need_to_close_paragraph = false;
	std::string code_style = "trac";
};

template<>
std::string Expansion<HTML_Manager>::to_string();

}

#endif
