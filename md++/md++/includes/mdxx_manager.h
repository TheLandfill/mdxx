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
	void print_expansion_flip();
	const std::vector<std::string>& context_stack() const;
	std::unique_ptr<Context>& cur_context();
	variable_map& cur_context_vars();
	std::string next_line();
	std::string next_line_no_nl();
	std::string print_line();
	void handle_context(HTML_Manager& html);
	void find_next_content_line();
	std::string expand_line(std::string & line);
	template<typename T>
	static void add_new_context(const std::string name, variable_map variables);
	static void add_function(const std::string name, gen_func function);
private:
	void variable_definition(std::string& line);
	void function_import(std::string& line);
	void immediate_substitution(std::string& line);
	void open_context(std::string& line, HTML_Manager& html);
	void close_context(std::string& line, HTML_Manager& html);
	void check_variable_dependency(const Context& c);

	void throw_exception_if_variable_not_found(const std::string& var);
private:
	std::ifstream& in;
	std::vector<std::string> context = { "default" };
	size_t line_number = 0;
	bool print_expansion = false;
	static std::unordered_map<std::string, std::unique_ptr<Context>> context_dict;
	static std::unordered_map<std::string, gen_func> imported_function_dict;
	std::string line_stack;
	Line_Data line_data;
};

}
#endif
