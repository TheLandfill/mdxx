#ifndef MDXX_MDXX_MANAGER_H
#define MDXX_MDXX_MANAGER_H
#include "expansion.h"
#include "context.h"
#include "line_data.h"
#include <unordered_map>
#include <memory>
#include <fstream>

namespace mdxx {

class MDXX_Manager {
public:
	MDXX_Manager(std::ifstream& i);
	MDXX_Manager(std::ifstream&& i);
	MDXX_Manager(std::string filename);
	void print_expansion_flip();
	const std::vector<std::string>& context_stack() const;
	std::unique_ptr<Context>& cur_context();
	variable_map& cur_context_vars();
	std::unique_ptr<Expansion_Base>& get_var(std::string variable);
	std::string next_line();
	std::string next_line_no_nl();
	std::string print_line();
	void handle_context(HTML_Manager& html);
	void find_next_content_line();
	std::string find_and_return_next_content_line();
	std::string expand_line(std::string & line);
	void set_context(std::vector<std::string> new_context);
	bool at_end_of_file();
	template<typename T>
	static void add_variable_to_context(const std::string& context, const std::string variable_name, T value);
	template<typename T>
	static void add_variable_to_context(const std::string& context, const std::string variable_name, T* value);
	template<typename T>
	static void add_new_context(const std::string name, variable_map variables);
	template<typename T>
	static void add_new_context(const std::string name);
	static void add_function(const std::string name, gen_func function);
	std::string list_all_vars();
	std::string list_context_stack();
	static std::string list_valid_contexts();
	static std::string list_imported_functions();
	~MDXX_Manager();
private:
	void variable_definition(std::string& line);
	void function_import(std::string& line);
	void immediate_substitution(std::string& line);
	void open_context(std::string& line, HTML_Manager& html);
	void close_context(std::string& line, HTML_Manager& html);
	void check_variable_dependency(const Context& c);

	std::string find_context_with_variable(const std::string& var);
	static void throw_exception_if_context_not_found(const std::string& context);
	void check_if_imported_function_found(const std::string& function);
private:
	std::ifstream* in_if_need_to_allocate = nullptr;
	std::ifstream& in;
	std::vector<std::string> context = { "default" };
	size_t line_number = 0;
	bool print_expansion = false;
	static std::unordered_map<std::string, std::unique_ptr<Context>> context_dict;
	static std::unordered_map<std::string, gen_func> imported_function_dict;
	std::string line_stack;
	Line_Data line_data;
	bool finished_reading = false;
};

template<typename T>
void MDXX_Manager::add_variable_to_context(const std::string& context, const std::string variable_name, T value) {
	throw_exception_if_context_not_found(context);
	context_dict[context]->add_variable(variable_name, value);
}

template<typename T>
void MDXX_Manager::add_variable_to_context(const std::string& context, const std::string variable_name, T* value) {
	throw_exception_if_context_not_found(context);
	context_dict[context]->add_variable(variable_name, value);
}

template<typename T>
void MDXX_Manager::add_new_context(const std::string name, variable_map variables) {
	context_dict.emplace(name, std::make_unique<T>(name, variables));
}

template<typename T>
void MDXX_Manager::add_new_context(const std::string name) {
	context_dict[name] = std::make_unique<T>(name);
}

}
#endif
