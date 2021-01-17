#include "html_lists.h"
#include "html_manager.h"

namespace mdxx {

HTML_List::HTML_List(const std::string name) :
	Context(name),
	list_first_element(true)
{}

void HTML_List::open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) {
	(void)args;
	(void)mdxx;
	std::string opening = std::string("<") + name + ">";
	html.add(opening);
	html.push();
	list_first_element = true;
}

void HTML_List::process(HTML_Manager& html, Line_Data& ls) {
	if (list_first_element) {
		html.add_no_nl("<li>");
		html.add_pre(ls.line);
		html.push();
		list_first_element = false;
		return;
	}
	if (ls.num_lines > 1) {
		html.pop();
		html.add("</li>");
		html.add_no_nl("<li>");
		html.add_pre(ls.line);
		html.push();
	} else {
		html.add(ls.line);
	}
}

void HTML_List::close(HTML_Manager& html) {
	html.pop();
	html.add("</li>");
	html.pop();
	html.add_no_nl("</");
	html.write(name);
	html.add_pre(">");
}

HTML_List::~HTML_List() {}

}
