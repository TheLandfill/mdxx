#ifndef MDXX_CONTEXT_H
#define MDXX_CONTEXT_H
#include "expansion.h"
#include "line_data.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

typedef std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> variable_map;

class MDXX_Manager;
class HTML_Manager;

class Context {
public:
	Context(std::string name, variable_map& v);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) const = 0;
	virtual void process(HTML_Manager& html, Line_Data& ls) const = 0;
	virtual void close(HTML_Manager& html) const = 0;
	variable_map& get_variables();
	template<typename T>
	void add_variable(std::string variable_name, T variable_value) {
		variables.insert({variable_name, make_expansion(variable_value)});
	}
protected:
	std::string name;
	variable_map variables;
};

}

#endif
