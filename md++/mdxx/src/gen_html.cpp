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

#include "gen_html.h"
#include "thread_safe_print.h"
#include "filesys_wrapper.h"
#include "html_manager.h"
#include "content_manager.h"
#include "template_manager.h"

void missing_file_error(const std::string& in_file_name) {
	std::string error_message;
	error_message.reserve(1024);
	error_message += "Input file\n\t";
	error_message += MDXX_FILE_COLOR;
	error_message += in_file_name;
	error_message += MDXX_ERROR_COLOR;
	error_message += "\ndoes not exist. No output generated.";
	error_message += MDXX_RESET;
	error_message += "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
	MDXX_error(nullptr, error_message.c_str());
}

extern "C" DLL_IMPORT_EXPORT bool MDXX_gen_html(const char * mdxx_file, const fs::path& template_path, mdxx::Plugin_Loader* pl) {
	using namespace mdxx;
	fs::path infile(fs::absolute(mdxx_file));
	if (!fs::exists(infile)) {
		missing_file_error(infile.string());
		return false;
	}
	std::ifstream in{infile};
	MDXX_Manager mdxx = MDXX_Manager(in);
	fs::path outfile = infile;
	outfile.replace_extension(".html");
	HTML_Manager html{outfile.string()};
	std::shared_ptr<Content_Manager> content = std::make_shared<Content_Manager>(html, mdxx, infile.string());
	std::string template_file = template_path.string() + mdxx.next_line_no_nl();
	Template_Manager template_reader(html, content, template_file);
	fs::path metafile = infile;
	metafile.replace_extension(".json");
	mdxx.add_variable_to_context("default", "metafile", c_string_copy(metafile.string().c_str()));
	mdxx.add_variable_to_context<Plugin_Loader*>("default", "plugin-obj", pl);
	template_reader.process_template();
	return (!mdxx.had_error() && !template_reader.had_error());
}
