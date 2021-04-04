// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
	HTML_Manager(std::string filename);
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
	void delete_outfile();
	HTML_Manager(const HTML_Manager& html) = delete;
	HTML_Manager& operator=(const HTML_Manager& html) = delete;
	HTML_Manager(const HTML_Manager&& html) = delete;
	HTML_Manager& operator=(const HTML_Manager&& html) = delete;
public:
	std::string html_object_id;
private:
	std::string tab_level = "";
	std::string outfile_name;
	std::ofstream out;
	bool need_to_close_paragraph = false;
	bool valid = true;
};

template<>
const char * Expansion<HTML_Manager>::to_string();

template<>
Expansion_Base* Expansion<HTML_Manager>::make_deep_copy();

template<>
const char * Expansion<HTML_Manager*>::to_string();

}

extern "C" DLL_IMPORT_EXPORT char * MDXX_html_push(mdxx::MDXX_Manager * md, mdxx::Expansion_Base** args, size_t argc);
extern "C" DLL_IMPORT_EXPORT char * MDXX_html_pop(mdxx::MDXX_Manager * md, mdxx::Expansion_Base** args, size_t argc);

extern "C" DLL_IMPORT_EXPORT void MDXX_html_add(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_add_pre(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_add_no_nl(mdxx::HTML_Manager* html, const char * line);
extern "C" DLL_IMPORT_EXPORT void MDXX_html_write(mdxx::HTML_Manager* html, const char * line);

#endif
