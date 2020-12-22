#include "template_manager.h"
#include <sstream>
#include <memory>

namespace mdxx {

std::string process_content(std::vector<std::unique_ptr<Expansion_Base>> args);

Template_Manager::Template_Manager(HTML_Manager& h, std::shared_ptr<Content_Manager> c, std::string template_path) :
	html(h),
	content(c),
	mdxx(content->get_mdxx()),
	template_name(template_path + mdxx.next_line_no_nl()),
	template_mdxx(
		std::ifstream(
			template_name
		)
	)
{
	template_mdxx.add_new_context<Template_Context>("template");
	template_mdxx.add_variable_to_context("template", "template", this);
	template_mdxx.add_variable_to_context("template", "content", this->content);
	template_mdxx.add_variable_to_context("template", "self", this->content);
	template_mdxx.add_variable_to_context("template", "html", html);
	template_mdxx.add_variable_to_context("template", "path", "");
	template_mdxx.add_variable_to_context("template", "plugin", load_plugins);
	template_mdxx.add_variable_to_context("template", "process_content", process_content);
	template_mdxx.add_variable_to_context("template", "switch_to_content", "{{process_content (content)}}");
	template_mdxx.add_variable_to_context("raw-html", "html", html);
	template_mdxx.set_context({"default", "template"});
}

std::string Template_Manager::load_plugins(std::vector<std::unique_ptr<Expansion_Base>> & args) {
	for (auto& arg : args) {
		std::string plugin_name = *static_cast<std::string*>(arg->get_data());
		plugin_loader.load_plugin(plugin_name.c_str());
	}
	return "";
}

void Template_Manager::process_template() {
	while (true) {
		std::string line = template_mdxx.find_and_return_next_content_line();
		if (line == "") {
			break;
		}
		template_mdxx.handle_context(html);
	}
}

std::string process_content(std::vector<std::unique_ptr<Expansion_Base>> args) {
	static_cast<Content_Manager*>(args.at(0)->get_data())->process_content();
	return "";
}

template<>
std::string Expansion<Template_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<Template_Manager object @ " << (void*)this << ">";
	return strstr.str();
}

Plugin_Loader Template_Manager::plugin_loader;

}
