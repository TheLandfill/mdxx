#ifndef MDXX_CONTEXT_H
#define MDXX_CONTEXT_H
#include "expansion.h"
#include "dll_info.h"
#include "variable_map.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

#define MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(X) \
mdxx::Expansion_Base* X::get_variable(const char * variable_name) { \
	return variables.at(std::string(variable_name)).get(); \
} \
\
bool X::check_if_var_exists(const char * variable_name) { \
	return variables.count(std::string(variable_name)) > 0; \
} \
\
const char * X::get_name() { \
	return name.c_str(); \
} \
\
void X::add_variable(const char * variable, std::unique_ptr<mdxx::Expansion_Base>&& value) { \
	variables[std::string(variable)] = std::move(value); \
} \
\
void X::add_variable(const char * variable, mdxx::gen_func func) {\
	variables[std::string(variable)] = std::make_unique<mdxx::Expansion<mdxx::gen_func>>(func, std::string(variable)); \
}\
\
void X::add_variable(const char * variable, const char * value) {\
	variables[std::string(variable)] = std::make_unique<mdxx::Expansion<std::string>>(value); \
}\
\
const char * X::list_variables_as_text() { \
	all_vars_as_text.clear(); \
	for (auto& vars_in_context : variables) { \
		all_vars_as_text += "\t"; \
		all_vars_as_text += vars_in_context.first; \
		all_vars_as_text += "  -->  "; \
		all_vars_as_text += vars_in_context.second->to_string(); \
		all_vars_as_text += "\n"; \
	} \
	return all_vars_as_text.c_str(); \
}

#define MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION \
public: \
const char * list_variables_as_text() override; \
mdxx::Expansion_Base* get_variable(const char * variable_name) override; \
bool check_if_var_exists(const char * variable_name) override; \
const char * get_name() override; \
void add_variable(const char * variable, std::unique_ptr<mdxx::Expansion_Base>&& value) override;\
template<typename T> \
void add_variable(const char * variable, T value) { \
	add_variable(variable, std::make_unique<mdxx::Expansion<T>>(value)); \
} \
template<typename T> \
void add_variable(const char * variable, T* value) { \
\
	add_variable(variable, std::make_unique<mdxx::Expansion<T*>>(value)); \
} \
void add_variable(const char * variable_name, mdxx::gen_func value); \
void add_variable(const char * variable_name, const char * value); \
private: \
	std::string name; \
	mdxx::variable_map variables; \
	std::string all_vars_as_text;

class HTML_Manager;

class DLL_IMPORT_EXPORT Context {
public:
	virtual void open(HTML_Manager& html, const char * args) = 0;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) = 0;
	virtual void close(HTML_Manager& html) = 0;
	virtual const char * list_variables_as_text() = 0;
	virtual Expansion_Base* get_variable(const char * variable_name) = 0;
	virtual bool check_if_var_exists(const char * variable_name) = 0;
	virtual void add_variable(const char * variable_name, std::unique_ptr<Expansion_Base>&& variable_value) = 0;
	virtual const char * get_name() = 0;
};

}

#endif
