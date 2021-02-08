#ifndef MDXX_CONTEXT_H
#define MDXX_CONTEXT_H
#include "expansion.h"
#include "line_data.h"
#include "dll_info.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

typedef std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> variable_map;

class HTML_Manager;

class DLL_IMPORT_EXPORT Context {
public:
	Context(const char * name);
	virtual void open(HTML_Manager& html, const char * args) = 0;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) = 0;
	virtual void close(HTML_Manager& html) = 0;
	variable_map& get_variables();
	template<typename T>
	void add_variable(const char * variable_name, T variable_value) {
		variables[std::string(variable_name)] = std::make_unique<Expansion<T>>(variable_value);
	}
	std::string get_name();
	virtual ~Context();
protected:
	std::string name;
	variable_map variables;
};

template<>
void Context::add_variable(const char * name, const char * variable_value);

template<>
void Context::add_variable<gen_func>(const char * variable_name, gen_func variable_value);

}

#endif
