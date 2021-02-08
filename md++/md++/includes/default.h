#ifndef MDXX_DEFAULT_H
#define MDXX_DEFAULT_H
#include "context.h"
#include <memory>
#include <unordered_map>

namespace mdxx {

class Default : public Context {
public:
	Default(const char * name);
	virtual void open(HTML_Manager& html, const char * args) override;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) override;
	virtual void close(HTML_Manager& html) override;
	~Default();
};

extern std::string empty_str;

}

#endif
