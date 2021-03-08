#include "template_manager.h"
#include "raw_html.h"
#include "default.h"
#include "html_lists.h"
#include "metadata.h"
#include <sstream>
#include <memory>
#include <iostream>
#include <stdexcept>

namespace mdxx {

char * process_content(Expansion_Base** args, size_t argc);

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
	template_mdxx.set_imported_function_dict(mdxx.get_imported_function_dict());
	mdxx.add_new_context<Template_Context>("template");
	mdxx.add_new_context<Raw_HTML>("raw-html");
	mdxx.add_new_context<Default>("default");
	mdxx.add_new_context<HTML_List>("ol");
	mdxx.add_new_context<HTML_List>("ul");
	mdxx.add_variable_to_context("template", "template", this);
	mdxx.add_variable_to_context("default", "content", content.get());
	mdxx.add_variable_to_context("default", "self", content.get());
	mdxx.add_variable_to_context("template", "path", "");
	mdxx.add_variable_to_context("default", "plugin-loader", MDXX_load_plugins);
	mdxx.add_variable_to_context("default", "plugin", "{{plugin-loader (plugin-obj) (mdxx) [1:]}}");
	mdxx.add_variable_to_context("default", "plugin-dir", MDXX_set_plugin_dir);
	mdxx.add_variable_to_context("template", "process_content", process_content);
	mdxx.add_variable_to_context("template", "switch_to_content", "{{process_content (content)}}");
	mdxx.add_variable_to_context("default", "html", &html);
	mdxx.add_variable_to_context("default", "mdxx", &mdxx);
	mdxx.add_variable_to_context("default", "open-func", MDXX_Manager::open_context);
	mdxx.add_variable_to_context("default", "close-func", MDXX_Manager::close_context);
	mdxx.add_variable_to_context("default", "open", "{{open-func [1] (mdxx) (html) [1:]}}");
	mdxx.add_variable_to_context("default", "close", "{{close-func [1] (mdxx) (html)}}");
	mdxx.add_variable_to_context("default", "metadata", MDXX_read_metadata_file);
	template_mdxx.set_context({"default", "template"});
}

void Template_Manager::process_template() {
	while (true) {
		std::string line = template_mdxx.find_and_return_next_content_line();
		if (template_mdxx.at_end_of_file()) {
			break;
		}
		template_mdxx.handle_context(html);
	}
	template_mdxx.destroy_contexts();
	mdxx.destroy_contexts();
}

char * process_content(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		throw std::runtime_error(
"process_content requires the an Expansion<std::shared_ptr<Content_Manager>*>>\n"
"as its argument."
		);
	}
	(*static_cast<std::shared_ptr<Content_Manager>*>(args[0]->get_data()))->process_content();
	return nullptr;
}

template<>
const char * Expansion<Template_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<Template_Manager object @ " << this->get_data() << ">";
	Template_Manager* tm = *static_cast<Template_Manager**>(this->get_data());
	tm->template_object_id = strstr.str();
	return tm->template_object_id.c_str();
}

}
