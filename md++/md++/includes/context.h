#ifndef MDXX_CONTEXT_H
#define MDXX_CONTEXT_H
#include "expansion.h"
#include "line_data.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

typedef std::unordered_map<std::string, Expansion> variable_map;

class MDXX_Manager;
class HTML_Manager;

typedef void (*open_context)(HTML_Manager& html, const std::vector<std::string>& args, MDXX_Manager& mdxx);
typedef void (*close_context)(HTML_Manager& html);
typedef void (*process_context)(HTML_Manager& html, Line_Data& ls);

class Context {
public:
	Context(std::string name, variable_map v);
	virtual void open(HTML_Manager& html, const std::vector<std::string>& args, MDXX_Manager& mdxx);
	virtual void process(HTML_Manager& html, Line_Data& ls);
	virtual void close(HTML_Manager& html);
protected:
	variable_map variables;
};

}

#endif
