#ifndef MDXX_CONTENT_MANAGER_H
#define MDXX_CONTENT_MANAGER_H
#include "html_manager.h"
#include "mdxx_manager.h"
#include "expansion.h"
#include <memory>

namespace mdxx {

class Content_Manager {
public:
	Content_Manager(HTML_Manager& html, MDXX_Manager& mdxx);
	virtual void process_content() = 0;
	MDXX_Manager& get_mdxx();
private:
	HTML_Manager& html;
	MDXX_Manager& mdxx;
};

std::shared_ptr<Content_Manager> get_content_manager(HTML_Manager& html, MDXX_Manager& mdxx);

template<>
std::string Expansion<std::shared_ptr<Content_Manager>>::to_string();

}

#endif
