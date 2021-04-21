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
#include <chrono>
#ifdef WIN32
#include <windows.h>
#include <iostream>
#define SETUP_ANSI_TERMINAL setup_ansi_terminal();
void setup_ansi_terminal() {
	DWORD l_mode;
	HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hstd == INVALID_HANDLE_VALUE) {
		std::cerr << "Won't be able to display ANSI terminal characters.\n";
		return;
	}
	GetConsoleMode(hstd, &l_mode);
	SetConsoleMode(hstd, l_mode | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

class UTF8CodePage {
public:
	UTF8CodePage() : m_old_code_page(::GetConsoleOutputCP()) {
		::SetConsoleOutputCP(CP_UTF8);
		::SetConsoleCP(CP_UTF8);
	}
	~UTF8CodePage() { ::SetConsoleOutputCP(m_old_code_page); ::SetConsoleCP(m_old_code_page); }

private:
	UINT m_old_code_page;
};
#include <io.h>
#include <fcntl.h>

#define SETUP_UTF_8_TERMINAL static UTF8CodePage use_utf8;\
_setmode(_fileno(stdout), _O_U8TEXT);\
_setmode(_fileno(stderr), _O_U8TEXT);
#else
#define SETUP_ANSI_TERMINAL
#define SETUP_UTF_8_TERMINAL
#endif
#include <iomanip>
void usage_message(const char * program_name);

#define MDXX_PROGRAM_NAME "md++"
#define MDXX_COPYRIGHT "©"
#ifdef WIN32
#undef MDXX_PROGRAM_NAME
#define MDXX_PROGRAM_NAME "mdxx.exe"
#endif

typedef std::chrono::system_clock sys_clock;

int main(int argc, char ** argv) {
	SETUP_ANSI_TERMINAL
	SETUP_UTF_8_TERMINAL
	auto now = sys_clock::now();
	std::time_t now_c = sys_clock::to_time_t(now);
	struct tm *parts = std::localtime(&now_c);
	std::wcout << MDXX_RESET << L"\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
		<< "              " << MDXX_BOLD << "md++ "
		<< MDXX_VAR_COLOR << MDXX_COPYRIGHT << " " << parts->tm_year + 1900
		<< MDXX_CONTEXT_COLOR << " Joseph Mellor "
		<< MDXX_RESET << "<" << MDXX_VAL_COLOR << "the.landfill.coding@gmail.com" << MDXX_RESET << ">" L"\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	if (argc < 3) {
		usage_message(MDXX_PROGRAM_NAME);
		std::wcout << "\x1b[0m";
		return 1;
	}
	std::wcout << "\n" << MDXX_VAR_COLOR << "Template Directory:\t" << MDXX_VAL_COLOR << argv[1] << "\n\n";
	for (int i = 2; i < argc; i++) {
		std::wcout << MDXX_LINE_COLOR << std::setfill(L' ') << std::setw(5) << i - 1 << ":\t" << MDXX_FILE_COLOR << argv[i] << MDXX_RESET << "\n";
	}
	std::wcout << std::endl;
	return MDXX_run_program(argc, argv);
}

void usage_message(const char * program_name) {
	std::cerr << program_name << " template_directory mdxx_file[s]\n";
}