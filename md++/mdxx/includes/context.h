#ifndef MDXX_CONTEXT_H
#define MDXX_CONTEXT_H
#include "expansion.h"
#include "dll_info.h"
#include "variable_map_definition.h"
#include "c_string_copy.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

#ifdef WIN32
#define MDXX_VAR_COLOR "\x1b[38;2;128;255;200m"
#define MDXX_VAL_COLOR "\x1b[38;2;255;153;255m"
#define MDXX_RESET "\x1b[m"
#else
#define MDXX_VAR_COLOR "\033[38;2;128;255;200m"
#define MDXX_VAL_COLOR "\033[38;2;255;153;255m"
#define MDXX_RESET "\033[m"
#endif

#ifdef MDXX_EXTERNAL_CONTEXT
#define MDXX_VARIABLE_MAP mdxx::variable_map* variables;
#define MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(X) \
bool X::can_allow_autosubs() {\
	return allow_autosubs; \
} \
mdxx::Expansion_Base* X::get_variable(const char * variable_name) { \
	return MDXX_get_variable(variables, variable_name); \
} \
\
bool X::check_if_var_exists(const char * variable_name) { \
	return MDXX_check_if_var_exists(variables, variable_name); \
} \
\
const char * X::get_name() { \
	return name.c_str(); \
} \
\
void X::add_variable(const char * variable, mdxx::Expansion_Base * value) { \
	MDXX_add_general_variable(variables, variable, value); \
} \
\
void X::add_variable(const char * variable, mdxx::gen_func func) {\
	MDXX_add_function_variable(variables, variable, func); \
}\
\
void X::add_variable(const char * variable, const char * value) {\
	MDXX_add_string_variable(variables, variable, value); \
}\
\
const char * X::list_variables_as_text() { \
	all_vars_as_text.clear(); \
	all_vars_as_text += "           Variable            ┃            Value\n"; \
	all_vars_as_text += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";\
	for (auto& vars_in_context : *variables) { \
		all_vars_as_text += "    "; \
		all_vars_as_text += MDXX_VAR_COLOR; \
		all_vars_as_text += vars_in_context.first.substr(0, 22); \
		if (vars_in_context.first.length() > 22) { \
			all_vars_as_text += "..."; \
		} \
		all_vars_as_text += MDXX_RESET; \
		all_vars_as_text += std::string(25 - vars_in_context.first.length(), ' '); \
		all_vars_as_text += "  ┃  "; \
		all_vars_as_text += MDXX_VAL_COLOR; \
		all_vars_as_text += vars_in_context.second->to_string(); \
		all_vars_as_text += MDXX_RESET; \
		all_vars_as_text += "\n"; \
	} \
	return all_vars_as_text.c_str(); \
}
#else
#define MDXX_VARIABLE_MAP mdxx::variable_map variables;
#define MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(X) \
bool X::can_allow_autosubs() {\
	return allow_autosubs; \
} \
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
void X::add_variable(const char * variable, mdxx::Expansion_Base * value) { \
	std::unique_ptr<Expansion_Base> temp_value(value); \
	variables[std::string(variable)] = std::move(temp_value); \
} \
\
void X::add_variable(const char * variable, mdxx::gen_func func) {\
	variables[std::string(variable)] = std::make_unique<mdxx::Expansion<mdxx::gen_func>>(func, std::string(variable)); \
}\
\
void X::add_variable(const char * variable, const char * value) {\
	variables[std::string(variable)] = std::make_unique<mdxx::Expansion<char *>>(c_string_copy(value)); \
}\
\
const char * X::list_variables_as_text() { \
	all_vars_as_text.clear(); \
	all_vars_as_text += "           Variable            ┃            Value\n"; \
	all_vars_as_text += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";\
	for (auto& vars_in_context : variables) { \
		all_vars_as_text += "    "; \
		all_vars_as_text += MDXX_VAR_COLOR; \
		all_vars_as_text += vars_in_context.first.substr(0, 22); \
		if (vars_in_context.first.length() > 22) { \
			all_vars_as_text += "..."; \
		} \
		all_vars_as_text += MDXX_RESET; \
		all_vars_as_text += std::string(25 - vars_in_context.first.length(), ' '); \
		all_vars_as_text += "  ┃  "; \
		all_vars_as_text += MDXX_VAL_COLOR; \
		all_vars_as_text += vars_in_context.second->to_string(); \
		all_vars_as_text += MDXX_RESET; \
		all_vars_as_text += "\n"; \
	} \
	return all_vars_as_text.c_str(); \
}
#endif

#define MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION \
public: \
bool can_allow_autosubs() override; \
const char * list_variables_as_text() override; \
mdxx::Expansion_Base* get_variable(const char * variable_name) override; \
bool check_if_var_exists(const char * variable_name) override; \
const char * get_name() override; \
void add_variable(const char * variable, mdxx::Expansion_Base * value) override;\
template<typename T> \
void add_variable(const char * variable, T value) { \
	add_variable(variable, new mdxx::Expansion<T>(value)); \
} \
template<typename T> \
void add_variable(const char * variable, T* value) { \
\
	add_variable(variable, new mdxx::Expansion<T*>(value)); \
} \
void add_variable(const char * variable_name, mdxx::gen_func value); \
void add_variable(const char * variable_name, const char * value); \
private: \
	std::string name; \
	MDXX_VARIABLE_MAP \
	std::string all_vars_as_text; \
public: \
	const bool allow_autosubs = false;

class HTML_Manager;

class DLL_IMPORT_EXPORT Context {
public:
	virtual void open(HTML_Manager& html, const char * args) = 0;
	virtual void process(HTML_Manager& html, const char * line, size_t num_lines) = 0;
	virtual void close(HTML_Manager& html) = 0;
	virtual const char * list_variables_as_text() = 0;
	virtual Expansion_Base* get_variable(const char * variable_name) = 0;
	virtual bool check_if_var_exists(const char * variable_name) = 0;
	virtual void add_variable(const char * variable_name, Expansion_Base * variable_value) = 0;
	virtual const char * get_name() = 0;
	virtual bool can_allow_autosubs() = 0;
};

}

#endif
