#include "run_program.h"
#ifdef WIN32
#include <windows.h>
#include <iostream>
#define SETUP_ANSI_TERMINAL setup_ansi_terminal();
void setup_ansi_terminal() {
	DWORD l_mode;
	HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hstd == INVALID_HANDLE_VALUE) {
		std::cout << "Won't be able to display ANSI terminal characters.\n";
		return;
	}
	GetConsoleMode(hstd, &l_mode);
	SetConsoleMode(hstd, l_mode | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
#define SETUP_ANSI_TERMINAL
#endif

int main(int argc, char ** argv) {
	SETUP_ANSI_TERMINAL
	return MDXX_run_program(argc, argv);
}
