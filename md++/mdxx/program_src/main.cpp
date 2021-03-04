#include "run_program.h"
#ifdef WIN32
#include <windows.h>
#define SETUP_ANSI_TERMINAL setup_ansi_terminal();
void setup_ansi_terminal() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		return;
	}
	SetConsoleMode(hStdin, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
#define SETUP_ANSI_TERMINAL
#endif

int main(int argc, char ** argv) {
	SETUP_ANSI_TERMINAL
	return MDXX_run_program(argc, argv);
}
