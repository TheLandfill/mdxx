#ifndef MDXX_LISTS_H
#define MDXX_LISTS_H
#include "context.h"

namespace mdxx {
class HTML_List : public Context {
public:
	HTML_List(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~HTML_List();
private:
	bool list_first_element;
};

}

#endif
