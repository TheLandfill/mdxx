#ifndef MDXX_DEFAULT_H
#define MDXX_DEFAULT_H
#include "context.h"
#include <memory>
#include <unordered_map>

namespace mdxx {

class Default : public Context {
public:
	Default(const std::string name);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) override;
	virtual void process(HTML_Manager& html, Line_Data& ls) override;
	virtual void close(HTML_Manager& html) override;
	~Default();
};

extern std::string empty_str;

}

#endif
