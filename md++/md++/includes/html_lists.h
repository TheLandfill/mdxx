#ifndef MDXX_LISTS_H
#define MDXX_LISTS_H
#include "context.h"

namespace mdxx {
class HTML_List : public Context {
public:
	HTML_List(const std::string name);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) override;
	virtual void process(HTML_Manager& html, Line_Data& ls) override;
	virtual void close(HTML_Manager& html) override;
private:
	bool list_first_element;
};

}

#endif
