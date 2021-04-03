#include "template_manager.h"
#include "default.h"
#include "html_lists.h"
#include "metadata.h"
#include "mdxx_get.h"
#include "clear_line.h"
#include "thread_safe_print.h"
#include <sstream>
#include <memory>
#include <iostream>
#include <stdexcept>

namespace mdxx {

char * process_content(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

Template_Manager::Template_Manager(HTML_Manager& h, std::shared_ptr<Content_Manager> c, std::string template_file) :
	html(h),
	content(c),
	mdxx(content->get_mdxx()),
	template_name(template_file),
	template_mdxx(
		template_name
	)
{
	template_mdxx.set_context_dict(mdxx.get_context_dict());
	mdxx.add_new_context<Template_Context>("template");
	mdxx.add_new_context<Default>("default");
	mdxx.add_new_context<HTML_List>("ol");
	mdxx.add_new_context<HTML_List>("ul");
	mdxx.add_variable_to_context("template", "template", this);
	mdxx.add_variable_to_context("template", "t-mdxx", &template_mdxx);
	mdxx.add_variable_to_context("default", "content", content.get());
	mdxx.add_variable_to_context("default", "self", content.get());
	mdxx.add_variable_to_context("template", "path", "");
	mdxx.add_variable_to_context("default", "plugin-loader", MDXX_load_plugins);
	mdxx.add_variable_to_context("default", "plugin", "{{plugin-loader (plugin-obj) [1:]}}");
	mdxx.add_variable_to_context("template", "t-plugin", "{{plugin-loader (plugin-obj) [1:]}}");
	mdxx.add_variable_to_context("template", "process_content", process_content);
	mdxx.add_variable_to_context("template", "switch_to_content", "{{process_content (content)}}");
	mdxx.add_variable_to_context("default", "html", &html);
	mdxx.add_variable_to_context("default", "mdxx", &mdxx);
	mdxx.add_variable_to_context("default", "open-func", MDXX_Manager::open_context);
	mdxx.add_variable_to_context("default", "close-func", MDXX_Manager::close_context);
	mdxx.add_variable_to_context("default", "open", "{{open-func [1] (html) [2:]}}");
	mdxx.add_variable_to_context("default", "close", "{{close-func [1] (html)}}");
	mdxx.add_variable_to_context("default", "metadata", MDXX_read_metadata_file);
	template_mdxx.set_context({"default", "template"});
}

void Template_Manager::process_template() {
	try {
		while (true) {
			std::string line = template_mdxx.find_and_return_next_content_line();
			if (template_mdxx.at_end_of_file()) {
				break;
			}
			template_mdxx.handle_context(html);
		}
	} catch (std::runtime_error& e) {
		MDXX_error(&template_mdxx, e.what());
	}
	if (template_mdxx.had_error()) {
		std::cerr << MDXX_CLEAR_LINE << MDXX_ERROR_COLOR << "\nERROR DETECTED" << MDXX_RESET << " in\n\t" << MDXX_FILE_COLOR << content->get_infile() << MDXX_RESET << "\nOutput will be deleted.\n"
			"--------------------------------------------------------------------------------" << std::endl;
		html.delete_outfile();
	}
	template_mdxx.destroy_contexts();
	mdxx.destroy_contexts();
}

bool Template_Manager::had_error() {
	return template_mdxx.had_error();
}

char * process_content(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		MDXX_error(mdxx, "process_content requires the a Content_Manager as its argument.");
	} else {
		MDXX_get<Content_Manager*>(args[0])->process_content();
	}
	return nullptr;
}

template<>
const char * Expansion<Template_Manager*>::to_string() {
	std::stringstream strstr;
	Template_Manager* tm = MDXX_GET(Template_Manager*, this);
	strstr << "<Template_Manager object @ " << tm << ">";
	tm->template_object_id = strstr.str();
	return tm->template_object_id.c_str();
}

}
