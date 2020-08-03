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
	Context(std::string name);
	virtual void open(HTML_Manager& html, std::string& args, MDXX_Manager& mdxx) = 0;
	virtual void process(HTML_Manager& html, Line_Data& ls) = 0;
	virtual void close(HTML_Manager& html) = 0;
	variable_map& get_variables();
	template<typename T>
	void add_variable(std::string variable_name, T variable_value) {
		variables.insert({variable_name, MAKE_EXPANSION(T, variable_value)});
	}
	std::string get_name();
protected:
	std::string name;
	variable_map variables;
};

template<>
void Context::add_variable<gen_func>(std::string variable_name, gen_func variable_value);

}

#endif
