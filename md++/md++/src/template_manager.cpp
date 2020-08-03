#include "template_manager.h"
#include "template_context.h"
#include <sstream>
#include <memory>

namespace mdxx {

Template_Manager::Template_Manager(HTML_Manager& h, Content_Manager& c, std::string template_path) :
	html(h),
	content(std::make_shared<Content_Manager>(std::move(c))),
	mdxx(content->get_mdxx()),
	template_name(template_path + mdxx.find_and_return_next_content_line()),
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
	template_mdxx.add_variable_to_context("raw-html", "html", html);
}

template<>
std::string Expansion<Template_Manager*>::to_string() {
	std::stringstream strstr;
	strstr << "<Template_Manager object @ " << (void*)this << ">";
	return strstr.str();
}

}
