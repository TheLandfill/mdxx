// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "run_program.h"
#include "mdxx_ansi.h"
#include "thread_safe_print.h"
#include <chrono>
#include <string>
#ifdef WIN32
#include <windows.h>
#define SETUP_ANSI_TERMINAL setup_ansi_terminal();
void setup_ansi_terminal() {
	DWORD l_mode;
	HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hstd == INVALID_HANDLE_VALUE) {
		MDXX_warn("Won't be able to display ANSI terminal characters.\n");
		return;
	}
	GetConsoleMode(hstd, &l_mode);
	SetConsoleMode(hstd, l_mode | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

#include <io.h>
#include <fcntl.h>

#define SETUP_UTF_8_TERMINAL \
_setmode(_fileno(stdout), _O_U8TEXT);\
_setmode(_fileno(stderr), _O_U8TEXT);
#else
#define SETUP_ANSI_TERMINAL
#define SETUP_UTF_8_TERMINAL
#endif
#include <iomanip>
void usage_message(const char * program_name);
void print_copyright_info();
void print_args(int argc, char ** argv);

#define MDXX_PROGRAM_NAME "md++"
#define MDXX_COPYRIGHT "©"
#ifdef WIN32
#undef MDXX_PROGRAM_NAME
#define MDXX_PROGRAM_NAME "mdxx.exe"
#endif

int main(int argc, char ** argv) {
	SETUP_ANSI_TERMINAL
	SETUP_UTF_8_TERMINAL
	print_copyright_info();
	if (argc < 3) {
		usage_message(MDXX_PROGRAM_NAME);
		return 1;
	}
	print_args(argc, argv);
	int prog_result = MDXX_run_program(argc, argv);
	return prog_result;
}

void usage_message(const char * program_name) {
	std::string usage_message;
	usage_message.reserve(128);
	usage_message += "USAGE: ";
	usage_message += program_name;
	usage_message += " template_directory mdxx_file[s]\x1b[0m\n";
	MDXX_print(stderr, usage_message.c_str());
}

void print_copyright_info() {
	std::string copyright_info;
	copyright_info.reserve(1024);
	copyright_info += MDXX_RESET
		"\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
		"              "
		MDXX_BOLD
		"md++ "
		MDXX_VAR_COLOR
		MDXX_COPYRIGHT
		" ";
	copyright_info += "2021";
	copyright_info += MDXX_CONTEXT_COLOR
		 " Joseph Mellor "
		 MDXX_RESET
		 "<"
		 MDXX_VAL_COLOR
		 "the.landfill.coding@gmail.com"
		 MDXX_RESET
		 ">"
		 "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	MDXX_print(stdout, copyright_info.c_str());
}

void print_args(int argc, char ** argv) {
	std::string output;
	output.reserve(2048);
	output += "\n" MDXX_VAR_COLOR "Template Directory:\t" MDXX_VAL_COLOR;
	output += argv[1];
	output += "\n\n";
	for (int i = 2; i < argc; i++) {
		std::string number = std::to_string(i - 1);
		output += MDXX_LINE_COLOR;
		output += (&"     " [ number.length()]);
		output += number;
		output += ":\t" MDXX_FILE_COLOR;
		output += argv[i];
		output += MDXX_RESET "\n";
	}
	output += "\n";
	MDXX_print(stdout, output.c_str());
}
