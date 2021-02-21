#ifndef MDXX_RAW_HTML_H
#define MDXX_RAW_HTML_H
#include "context.h"

namespace mdxx {
class Raw_HTML : public Context {
public:
	Raw_HTML(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * args, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~Raw_HTML();
private:
	bool in_pre_section = false;
MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

}

#endif
