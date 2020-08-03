#ifndef MDXX_TEMPLATE_MANAGER_H
#define MDXX_TEMPLATE_MANAGER_H
#include "html_manager.h"
#include "mdxx_manager.h"
#include "content_manager.h"
#include <memory>

namespace mdxx {

class Template_Manager {
public:
	Template_Manager(HTML_Manager& html, Content_Manager& content, std::string template_path);
private:
	HTML_Manager& html;
	std::shared_ptr<Content_Manager> content;
	MDXX_Manager& mdxx;
	std::string template_name;
	MDXX_Manager template_mdxx;
};

template<>
std::string Expansion<Template_Manager*>::to_string();

}

#endif
