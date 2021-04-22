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
#include <locale>
#include <stdexcept>
#ifdef WIN32
#include <Windows.h>
#endif

bool MDXX_error(mdxx::MDXX_Manager* md, const char * str) {
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
	bool print_success = MDXX_print(stderr, output.c_str());
	if (md != nullptr) {
		MDXX_print_current_line_and_exit(md);
	}
	return print_success;
}

bool MDXX_warn(const char * str) {
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
	return MDXX_print(stderr, output.c_str());
}

#ifdef WIN32
static void identify_windows_error() {
	LPWSTR err_str;
	DWORD last_err = GetLastError();
	switch (last_err) {
	case ERROR_INSUFFICIENT_BUFFER:
		err_str = L"INSUFFICIENT BUFFER";
		break;
	case ERROR_INVALID_FLAGS:
		err_str = L"INVALID FLAGS";
		break;
	case ERROR_INVALID_PARAMETER:
		err_str = L"INVALID PARAMETER";
		break;
	case ERROR_NO_UNICODE_TRANSLATION:
		err_str = L"NO UNICODE TRANSLATION";
		break;
	default:
		err_str = L"UNKNOWN ERROR";
	}
	fwprintf(stderr, L"ERROR:       %s\nERROR CODE: %ld", err_str, last_err);
}
#endif

bool MDXX_print(FILE* out, const char * str) {
	#pragma omp critical(thread_safe_printing)
	{
#ifdef WIN32
		int buff_length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
		if (buff_length < 0) {
			std::wstring error_message;
			fwprintf(stderr, L"%s",
L"The message that was supposed to be printed out cannot be printed for some\n"
L"reason. Contact the developers of the plugins in use.\n"
			);
			identify_windows_error();
			return false;
		}
		LPWSTR buffer;
		try {
			buffer = new WCHAR[buff_length];
		} catch (std::bad_alloc& e) {
			std::string cs(e.what());
			std::wstring ws;
			std::copy(cs.begin(), cs.end(), std::back_inserter(ws));
			fwprintf(stderr, L"%s\nException: %s\n",
				L"Could not allocate enough memory to print out the error message that was\n"
				L"supposed to be printed out.",
				ws.c_str()
			);
			identify_windows_error();
			return false;
		}
		int err_code = MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, buff_length);
		if (err_code == 0) {
			fwprintf(stderr, L"%s",
L"Unable to convert message to print to wide characters. Contact the developers\n"
L"of the plugins in use.\n"
			);
			identify_windows_error();
			return false;
		}
		fwprintf(out, L"%s", buffer);
		delete[] buffer;
#else
		fprintf(out, "%s", str);
#endif
		return true;
	}
}
