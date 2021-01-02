#include "template_manager.h"
#include "raw_html.h"
#include "default.h"
#include <sstream>
#include <memory>
#include <iostream>

namespace mdxx {

std::string process_content(std::vector<std::unique_ptr<Expansion_Base>>& args);

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
	MDXX_Manager::add_variable_to_context("template", "template", this);
	MDXX_Manager::add_variable_to_context("default", "content", content.get());
	MDXX_Manager::add_variable_to_context("default", "self", content.get());
	MDXX_Manager::add_variable_to_context<std::string>("template", "path", "");
	MDXX_Manager::add_variable_to_context("default", "plugin", load_plugins);
	MDXX_Manager::add_variable_to_context<gen_func>("default", "plugin-dir", Plugin_Loader::set_plugin_dir);
	MDXX_Manager::add_variable_to_context("template", "process_content", process_content);
	MDXX_Manager::add_variable_to_context<std::string>("template", "switch_to_content", "{{process_content (content)}}");
	MDXX_Manager::add_variable_to_context("default", "html", &html);
	MDXX_Manager::add_variable_to_context("default", "mdxx", &mdxx);
	MDXX_Manager::add_variable_to_context("default", "open_func", MDXX_Manager::open_context);
	MDXX_Manager::add_variable_to_context("default", "close_func", MDXX_Manager::close_context);
	MDXX_Manager::add_variable_to_context<std::string>("default", "new-context", "INVALID");
	MDXX_Manager::add_variable_to_context<std::string>("default", "open", "{{open_func (new-context) (mdxx) (html)}}");
	MDXX_Manager::add_variable_to_context<std::string>("default", "close", "{{close_func (new-context) (mdxx) (html)}}");
	template_mdxx.set_context({"default", "template"});
}

std::string Template_Manager::load_plugins(std::vector<std::unique_ptr<Expansion_Base>> & args) {
	std::cout << "Loading plugins\n";
	for (auto& arg : args) {
		std::string plugin_name = *static_cast<std::string*>(arg->get_data());
		plugin_loader.load_plugin(plugin_name.c_str());
	}
	return "";
}

void Template_Manager::process_template() {
	while (true) {
		std::string line = template_mdxx.find_and_return_next_content_line();
		if (template_mdxx.at_end_of_file()) {
			break;
		}
		template_mdxx.handle_context(html);
	}
	std::cout << "Finished processing template" << std::endl;
}

std::string process_content(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	(*static_cast<std::shared_ptr<Content_Manager>*>(args.at(0)->get_data()))->process_content();
	std::cout << "Finished processing content" << std::endl;
	return "";
}

template<>
std::string Expansion<Template_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<Template_Manager object @ " << this->get_data() << ">";
	return strstr.str();
}

Plugin_Loader Template_Manager::plugin_loader;

}
