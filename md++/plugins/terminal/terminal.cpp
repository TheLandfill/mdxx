#include "context.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include <string>
#include <iostream>

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
		add_variable<std::string>("ol", "<span class=\"terminal-oneline\">");
		add_variable<std::string>("prompt", "{{ol}}{{user-and-comp}}:{{full-dir}}$ {{command}}</span>");
		add_variable<std::string>("mac-prompt", "{{ol}}{{computer-name}}:{{mac-dir}} {{user}}$ {{command}}</span>");
	}
	void open(mdxx::HTML_Manager& html, std::string& args, mdxx::MDXX_Manager& mdxx) override {
		(void)mdxx;
		if (args.length() >= 3 && args.substr(0, 3) == "mac") {
			std::string opening = std::string("<div class=\"mac-terminal\"") + args.substr(3) + "><pre>";
			html.add_no_nl(opening);
		} else {
			std::string opening = std::string("<div class=\"terminal\"") + args + "><pre>";
			html.add_no_nl(opening);
		}
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
};

extern "C" void import_plugin() {
	mdxx::MDXX_Manager::add_new_context<Terminal>("terminal");
	std::cout << "Terminal imported.\n";
}
