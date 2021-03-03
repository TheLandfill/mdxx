#ifndef MDXX_HTML_MANAGER_H
#define MDXX_HTML_MANAGER_H
#include "expansion.h"
#include "dll_info.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

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
	HTML_Manager(const HTML_Manager& html) = delete;
	HTML_Manager& operator=(const HTML_Manager& html) = delete;
	HTML_Manager(const HTML_Manager&& html) = delete;
	HTML_Manager& operator=(const HTML_Manager&& html) = delete;
public:
	std::string html_object_id;
private:
	std::string tab_level = "";
	std::ofstream& out;
	bool need_to_close_paragraph = false;
	std::string code_style = "trac";
};

template<>
const char * Expansion<HTML_Manager>::to_string();

template<>
Expansion_Base* Expansion<HTML_Manager>::make_deep_copy();

template<>
const char * Expansion<HTML_Manager*>::to_string();

}

extern "C" DLL_IMPORT_EXPORT char * MDXX_html_push(mdxx::Expansion_Base** args, size_t argc);
extern "C" DLL_IMPORT_EXPORT char * MDXX_html_pop(mdxx::Expansion_Base** args, size_t argc);

extern "C" DLL_IMPORT_EXPORT void MDXX_html_add(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_add_pre(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_add_no_nl(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_write(mdxx::HTML_Manager* html, const char * line);

#endif
