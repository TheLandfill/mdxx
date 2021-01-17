#include "expansion.h"
#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include "compilation_info.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>

class Terminal : public mdxx::Context {
public:
	Terminal(const std::string name) : Context(name) {
		add_variable<std::string>("user", "user");
		add_variable<std::string>("computer-name", "computer");
		add_variable<std::string>("mac-dir", "~");
		add_variable<std::string>("dir", "~/dev");
		add_variable<std::string>("u-and-c-color", "#4CE64C");
		add_variable<std::string>("dir-color", "#298FDD");
		add_variable<std::string>("user-and-comp", "<b><span style=\"color: {{u-and-c-color}};\">{{user}}@{{computer-name}}</span></b>");
		add_variable<std::string>("full-dir", "<b><span style=\"color: {{dir-color}};\">{{dir}}</span></b>");
		add_variable<std::string>("command", "");
		add_variable<std::string>("oneline", "<span class=\"terminal-oneline\">");
		add_variable<mdxx::gen_func>("prompt", Terminal::prompt);
		add_variable<mdxx::gen_func>("mac-prompt", Terminal::mac_prompt);
	}
	void open(mdxx::HTML_Manager& html, std::string& args, mdxx::MDXX_Manager& mdxx) override {
		(void)mdxx;
		std::string opening;
		if (args.length() >= 3 && args.substr(0, 3) == "mac") {
			opening = std::string("<div class=\"mac-terminal\"") + args.substr(3) + "><pre>";
		} else {
			opening = std::string("<div class=\"terminal\"") + args + "><pre>";
		}
		html.add_no_nl(opening);
	}
	void process(mdxx::HTML_Manager& html, mdxx::Line_Data& ls) override {
		for (size_t i = 1; i < ls.num_lines; i++) {
			html.add_pre("");
		}
		html.add_pre(ls.line);
	}
	void close(mdxx::HTML_Manager& html) override {
		html.add_pre("</pre>");
		html.add("</div>");
	}
	static std::string collect_args(std::vector<std::unique_ptr<mdxx::Expansion_Base>>& args) {
		std::string command;
		command.reserve(128);
		for (auto& arg : args) {
			command += *static_cast<std::string*>(arg->get_data());
			command += " ";
		}
		command.pop_back();
		return command;
	}
	static std::string prompt(std::vector<std::unique_ptr<mdxx::Expansion_Base>>& args) {
		mdxx::MDXX_Manager::add_variable_to_context<std::string>("terminal", "command", collect_args(args));
		return "{{oneline}}{{user-and-comp}}:{{full-dir}}$ {{command}}</span>";
	}
	static std::string mac_prompt(std::vector<std::unique_ptr<mdxx::Expansion_Base>>& args) {
		mdxx::MDXX_Manager::add_variable_to_context<std::string>("terminal", "command", collect_args(args));
		return "{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$ {{command}}</span>";
	}
	~Terminal() {}
};

extern "C" void import_plugin() {
	mdxx::MDXX_Manager::add_new_context<Terminal>("terminal");
}

extern "C" void print_compilation_info() {
	std::cout << "\r\033[Kterminal:\t" << MDXX_COMPILATION_INFO << ".\n";
}
