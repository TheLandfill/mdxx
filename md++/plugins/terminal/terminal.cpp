#define WINDOWS_PLUGIN
#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

class DLL_IMPORT_EXPORT Terminal : public mdxx::Context {
public:
	Terminal(const char * n) : name(n) {
		add_variable("user", "user");
		add_variable("computer-name", "computer");
		add_variable("mac-dir", "~");
		add_variable("dir", "~/dev");
		add_variable("u-and-c-color", "#4CE64C");
		add_variable("dir-color", "#298FDD");
		add_variable("user-and-comp", "<b><span style=\"color: {{u-and-c-color}};\">{{user}}@{{computer-name}}</span></b>");
		add_variable("full-dir", "<b><span style=\"color: {{dir-color}};\">{{dir}}</span></b>");
		add_variable("command", "");
		add_variable("oneline", "<span class=\"terminal-oneline\">");
		add_variable<mdxx::gen_func>("prompt", Terminal::prompt);
		add_variable<mdxx::gen_func>("mac-prompt", Terminal::mac_prompt);
	}
	void open(mdxx::HTML_Manager& html, const char * arg_ptr) override {
		std::string args(arg_ptr);
		std::string opening;
		if (args.length() >= 3 && args.substr(0, 3) == "mac") {
			opening = std::string("<div class=\"mac-terminal\"") + args.substr(3) + "><pre>";
		} else {
			opening = std::string("<div class=\"terminal\"") + args + "><pre>";
		}
		html.add_no_nl(opening);
	}
	void process(mdxx::HTML_Manager& html, const char * line, size_t num_lines) override {
		for (size_t i = 1; i < num_lines; i++) {
			html.add_pre("");
		}
		html.add_pre(line);
	}
	void close(mdxx::HTML_Manager& html) override {
		html.add_pre("</pre>");
		html.add("</div>");
	}
	static std::string collect_args(mdxx::Expansion_Base** args, size_t argc) {
		std::string command;
		command.reserve(128);
		for (size_t i = 0; i < argc; i++) {
			command += *static_cast<const char **>(args[i]->get_data());
			command += " ";
		}
		command.pop_back();
		return command;
	}
	static char * prompt(mdxx::Expansion_Base** args, size_t argc) {
		mdxx::MDXX_Manager::add_variable_to_context<std::string>("terminal", "command", collect_args(args, argc));
		const char * temp = "{{oneline}}{{user-and-comp}}:{{full-dir}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	static char * mac_prompt(mdxx::Expansion_Base** args, size_t argc) {
		mdxx::MDXX_Manager::add_variable_to_context<std::string>("terminal", "command", collect_args(args, argc));
		const char * temp = "{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	~Terminal() {}
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};
//
//mdxx::Expansion_Base* Terminal::get_variable(const char * variable_name) { 
//	return variables.at(std::string(variable_name)).get(); 
//} 
//
//bool Terminal::check_if_var_exists(const char * variable_name) { 
//	return variables.count(std::string(variable_name)) > 0; 
//} 
//
//const char * Terminal::get_name() { 
//	return name.c_str(); 
//} 
//
//void Terminal::add_variable(const char * variable, std::unique_ptr<mdxx::Expansion_Base>&& value) { 
//	variables[std::string(variable)] = std::move(value); 
//} 
//
//template<>
//void Terminal::add_variable(const char * variable, mdxx::gen_func func) {
//	variables[std::string(variable)] = std::move(std::make_unique<mdxx::Expansion<mdxx::gen_func>>(func, std::string(variable))); 
//}
//
//template<>
//void Terminal::add_variable(const char * variable, const char * value) {
//	variables[std::string(variable)] = std::move(std::make_unique<mdxx::Expansion<std::string>>(value)); 
//}
//
//const char * Terminal::list_variables_as_text() { 
//	all_vars_as_text.clear(); 
//	for (auto& vars_in_context : variables) { 
//		all_vars_as_text += "\t"; 
//		all_vars_as_text += vars_in_context.first; 
//		all_vars_as_text += "  -->  "; 
//		all_vars_as_text += vars_in_context.second->to_string(); 
//		all_vars_as_text += "\n"; 
//	} 
//	return all_vars_as_text.c_str(); 
//}

extern "C" void import_plugin() {
	mdxx::MDXX_Manager::add_new_context<Terminal>("terminal");
}

extern "C" void print_compilation_info() {
	std::cout << "\r\033[Kterminal:\t" << MDXX_COMPILATION_INFO << ".\n";
}
