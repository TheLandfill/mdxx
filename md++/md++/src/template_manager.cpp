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
	MDXX_Manager::add_new_context<Template_Context>("template");
	MDXX_Manager::add_new_context<Raw_HTML>("raw-html");
	MDXX_Manager::add_new_context<Default>("default");
	MDXX_Manager::add_new_context<HTML_List>("ol");
	MDXX_Manager::add_new_context<HTML_List>("ul");
	MDXX_Manager::add_variable_to_context("template", "template", this);
	MDXX_Manager::add_variable_to_context("default", "content", content.get());
	MDXX_Manager::add_variable_to_context("default", "self", content.get());
	MDXX_Manager::add_variable_to_context("template", "path", "");
	MDXX_Manager::add_variable_to_context("default", "plugin", load_plugins);
	MDXX_Manager::add_variable_to_context("default", "plugin-dir", MDXX_set_plugin_dir);
	MDXX_Manager::add_variable_to_context("template", "process_content", process_content);
	MDXX_Manager::add_variable_to_context("template", "switch_to_content", "{{process_content (content)}}");
	MDXX_Manager::add_variable_to_context("default", "html", &html);
	MDXX_Manager::add_variable_to_context("default", "mdxx", &mdxx);
	MDXX_Manager::add_variable_to_context("default", "open-func", MDXX_Manager::open_context);
	MDXX_Manager::add_variable_to_context("default", "close-func", MDXX_Manager::close_context);
	MDXX_Manager::add_variable_to_context("default", "open", "{{open-func [1] (mdxx) (html)}}");
	MDXX_Manager::add_variable_to_context("default", "close", "{{close-func [1] (mdxx) (html)}}");
	MDXX_Manager::add_variable_to_context("default", "metadata", MDXX_read_metadata_file);
	template_mdxx.set_context({"default", "template"});
}

char * Template_Manager::load_plugins(Expansion_Base** args, size_t argc) {
	for (size_t i = 0; i < argc; i++) {
		std::string plugin_name = *static_cast<const char**>(args[i]->get_data());
		plugin_loader.load_plugin(plugin_name.c_str());
	}
	return nullptr;
}

void Template_Manager::process_template() {
	while (true) {
		std::string line = template_mdxx.find_and_return_next_content_line();
		if (template_mdxx.at_end_of_file()) {
			break;
		}
		template_mdxx.handle_context(html);
	}
	MDXX_Manager::destroy_contexts();
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

Plugin_Loader Template_Manager::plugin_loader;

}
