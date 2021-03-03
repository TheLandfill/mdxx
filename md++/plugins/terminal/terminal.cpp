#define MDXX_WINDOWS_PLUGIN
#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include "plugin_loader.h"
#include "variable_map.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

class Terminal;

template<>
const char * mdxx::Expansion<Terminal*>::to_string();

class Terminal : public mdxx::Context {
public:
	Terminal(const char * n, mdxx::Plugin_Loader * p) : name(n), pl(p) {
		variables = MDXX_get_variable_map(pl, this);
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
		add_variable("prompt-func", Terminal::prompt);
		add_variable("mac-prompt-func", Terminal::mac_prompt);
		add_variable("prompt", "{{prompt-func (self) [1:]}}");
		add_variable("mac-prompt", "{{mac-prompt-func (self) [1:]}}");
		MDXX_add_general_variable(variables, "self", new mdxx::Expansion<Terminal*>(this));
	}
	void open(mdxx::HTML_Manager& html, const char * arg_ptr) override {
		std::string args(arg_ptr);
		std::string opening;
		if (args.length() >= 3 && args.substr(0, 3) == "mac") {
			opening = std::string("<div class=\"mac-terminal\"") + args.substr(3) + "><pre>";
		} else {
			opening = std::string("<div class=\"terminal\"") + args + "><pre>";
		}
		MDXX_html_add_no_nl(&html, opening.c_str());
	}
	void process(mdxx::HTML_Manager& html, const char * line, size_t num_lines) override {
		for (size_t i = 1; i < num_lines; i++) {
			MDXX_html_add_pre(&html, "");
		}
		MDXX_html_add_pre(&html, line);
	}
	void close(mdxx::HTML_Manager& html) override {
		MDXX_html_add_pre(&html, "</pre>");
		MDXX_html_add(&html, "</div>");
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
		Terminal * term = *static_cast<Terminal**>(args[0]->get_data());
		MDXX_add_string_variable(term->variables, "command", collect_args(args + 1, argc - 1).c_str());
		const char * temp = "{{oneline}}{{user-and-comp}}:{{full-dir}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	static char * mac_prompt(mdxx::Expansion_Base** args, size_t argc) {
		Terminal * term = *static_cast<Terminal**>(args[0]->get_data());
		MDXX_add_string_variable(term->variables, "command", collect_args(args + 1, argc - 1).c_str());
		const char * temp = "{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	~Terminal() {
		MDXX_free_variable_map(pl, this);
	}
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
public:
	std::string term_object_id;
private:
	mdxx::Plugin_Loader * pl;
};

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Terminal)

extern "C" void import_plugin(mdxx::Plugin_Loader * pl, mdxx::MDXX_Manager * mdxx) {
	MDXX_add_new_context(mdxx, "terminal", new Terminal("terminal", pl));
}

extern "C" void print_compilation_info() {
	std::cout << "\r\033[Kterminal:\t" << MDXX_COMPILATION_INFO << ".\n";
}

template<>
const char * mdxx::Expansion<Terminal*>::to_string() {
	std::stringstream strstr;
	strstr << "<Terminal object @ " << data << ">";
	data->term_object_id = strstr.str();
	return data->term_object_id.c_str();
}