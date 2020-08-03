#ifndef MDXX_TEMPLATE_CONTEXT_H
#define MDXX_TEMPLATE_CONTEXT_H
#include "context.h"

namespace mdxx {

class Template_Context : public Context {
public:
	Template_Context(const std::string name);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) override;
	virtual void process(HTML_Manager& html, Line_Data& ls) override;
	virtual void close(HTML_Manager& html) override;
private:
	bool in_pre_section;
};

}

#endif
