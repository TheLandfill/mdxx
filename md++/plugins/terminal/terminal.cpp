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
		add_variable("prompt", Terminal::prompt);
		add_variable("mac-prompt", Terminal::mac_prompt);
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
		mdxx::MDXX_Manager::add_variable_to_context("terminal", "command", collect_args(args, argc));
		const char * temp = "{{oneline}}{{user-and-comp}}:{{full-dir}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	static char * mac_prompt(mdxx::Expansion_Base** args, size_t argc) {
		mdxx::MDXX_Manager::add_variable_to_context("terminal", "command", collect_args(args, argc));
		const char * temp = "{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$ {{command}}</span>";
		char * output = new char[strlen(temp) + 1];
		strncpy(output, temp, strlen(temp) + 1);
		return output;
	}
	~Terminal() {}
	MDXX_CONTEXT_COMMON_FUNCTIONALITY_DECLARATION
};

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(Terminal)

extern "C" void import_plugin() {
	mdxx::MDXX_Manager::add_new_context<Terminal>("terminal");
}

extern "C" void print_compilation_info() {
	std::cout << "\r\033[Kterminal:\t" << MDXX_COMPILATION_INFO << ".\n";
}
