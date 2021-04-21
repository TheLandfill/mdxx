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

#include "thread_safe_print.h"
#include "mdxx_manager.h"
#include "mdxx_ansi.h"
#include <string>
#include <cstring>
#include <iostream>

void MDXX_error(mdxx::MDXX_Manager* md, const char * str) {
	std::string output;
	output.reserve(strlen(str) + 128);
	output += "\n";
	output += MDXX_ERROR_COLOR;
	output += MDXX_ERROR_PREFIX;
	output += MDXX_RESET_FORMATTING;
	output += " ";
	output += MDXX_ERROR_COLOR;
	output += str;
	output += MDXX_RESET;
	output += "\n";
	MDXX_thread_safe_print(stderr, output.c_str());
	if (md != nullptr) {
		MDXX_print_current_line_and_exit(md);
	}
}

void MDXX_warn(const char * str) {
	std::string output;
	output.reserve(strlen(str) + 128);
	output += "\n";
	output += MDXX_WARNING_COLOR;
	output += MDXX_WARNING_PREFIX;
	output += MDXX_RESET_FORMATTING;
	output += " ";
	output += MDXX_WARNING_COLOR;
	output += str;
	output += MDXX_RESET;
	output += "\n";
	MDXX_thread_safe_print(stderr, output.c_str());
}

void MDXX_thread_safe_print(FILE* out, const char * str) {
	#pragma omp critical(thread_safe_printing)
	{
#ifdef WIN32
		if (out == stderr) {
			std::wcerr << str;
		} else if (out == stdout) {
			std::wcout << str;
		} else {
			fprintf(out, "%s", str);
			std::cerr << "Should not have run.\n";
		}
#else
		fprintf(out, "%s", str);
#endif
	}
}
