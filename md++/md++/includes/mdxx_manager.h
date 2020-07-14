#ifndef MDXX_MDXX_MANAGER_H
#define MDXX_MDXX_MANAGER_H
#include "expansion.h"
#include "context.h"
#include "line_data.h"
#include <unordered_map>
#include <fstream>

namespace mdxx {

class MDXX_Manager {
public:
	MDXX_Manager(std::ifstream& i);
	void print_expansion_flip();
	const variable_map& cur_context_vars();
	std::vector<std::string> cur_context();
	std::string next_line();
	std::string next_line_no_nl();
	void print_line();
	void handle_context(HTML_Manager& html);
	void find_next_content_line();
	std::string expand_line();
private:
	void variable_definition(const std::string& line);
	void function_import(const std::string& line);
	void immediate_substitution(const std::string& line);
	void open_context(const std::string& line, HTML_Manager& html);
	void close_context(std::string line, HTML_Manager& html);
	void check_variable_dependency(const Context& c);
private:
	std::ifstream& in;
	std::vector<std::string> context = { "default" };
	size_t line_number = 0;
	bool print_expansion = false;
	variable_map shared_context_dict;
	std::unordered_map<std::string, variable_map> context_dict;
	std::string line_stack;
	Line_Data line_data;
};

}
#endif
