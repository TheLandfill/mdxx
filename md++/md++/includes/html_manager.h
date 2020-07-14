#ifndef MDXX_HTML_MANAGER_H
#define MDXX_HTML_MANAGER_H
#include <fstream>
#include <string>

namespace mdxx {

class HTML_Manager {
public:
	HTML_Manager(std::ofstream& o);
	void add(std::string& line);
	void add_pre(std::string& line);
	void add_no_nl(std::string& line);
	void write(const std::string line);
	void write_empty(std::string str);
	void remove_empty(std::string& line);
	void check_and_close_paragraph();
	void push();
	void pop();
private:
	std::string tab_level = "";
	std::ofstream& out;
	bool need_to_close_paragraph = false;
	std::string code_style = "trac";
};

}

#endif
