#ifndef MDXX_RAW_HTML_H
#define MDXX_RAW_HTML_H
#include "context.h"

namespace mdxx {
class Raw_HTML : public Context {
public:
	Raw_HTML(const std::string name);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) override;
	virtual void process(HTML_Manager& html, Line_Data& ls) override;
	virtual void close(HTML_Manager& html) override;
	~Raw_HTML();
private:
	bool in_pre_section = false;
};

}

#endif
