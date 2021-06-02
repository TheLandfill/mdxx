#ifndef MDXX_VARIABLE_PARSER_H
#define MDXX_VARIABLE_PARSER_H
#include "mdxx_token.h"
#include "expansion.h"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

struct Range_Sub_Token {
	Range_Sub_Token(bool, int, int);
	Range_Sub_Token(bool, size_t, size_t);
	bool is_sub;
	union {
		int start;
		size_t begin;
	};
	union {
		int stop;
		size_t end;
	};
};

class MDXX_Manager;

class Parser {
public:
	Parser(MDXX_Manager * m, size_t depth);
	const std::string& parse(const char * line);
private:
	bool push();
	bool pop();
	void recursion_error();
	void sub_parse(const char * line, std::string& output);
	void expand_tokens();
	void reserve_output_length(std::string& output);
	void fill_output(std::string& output);
	void parse_line_for_arg_subs(const std::string_view& char_line, std::vector<Range_Sub_Token>& output);
	void handle_args_subs(const std::string_view& char_line);
	std::string handle_args(const std::string_view& token_str);
	std::vector<MDXX_Token>& cur_depth();
	std::string& cur_variable();
	std::string parsing_trace();
private:
	bool error_in_parsing = false;
	size_t cur_index = 0;
	size_t max_depth = 16;
	std::string result;
	std::unordered_map<std::string, std::string> cache;
	std::vector<MDXX_Token> arg_subs;
	std::vector<std::vector<MDXX_Token>> stack;
	std::vector<std::string> variable_stack;
	std::vector<std::unique_ptr<Expansion_Base>> c_args_to_clean;
	std::vector<Expansion_Base *> c_args;
	std::string current_arg;
	std::string range_string;
	std::string token_string;
	MDXX_Manager * md;
};

void parse_line_for_variables(MDXX_Manager *  md, const char * line, std::vector<MDXX_Token>& output);

}

#endif
