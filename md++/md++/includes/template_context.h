#ifndef MDXX_TEMPLATE_CONTEXT_H
#define MDXX_TEMPLATE_CONTEXT_H
#include "context.h"

namespace mdxx {

class Template_Context : public Context {
public:
	Template_Context(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * ls, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~Template_Context();
private:
	bool in_pre_section;
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

}

#endif
