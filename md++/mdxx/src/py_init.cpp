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

#include "py_init.h"
#include "thread_safe_print.h"
#include "mdxx_ansi.h"
#include <Python.h>
#include <iostream>

namespace mdxx {
	extern char * main_program_dir;
}

static bool python_has_been_initialized = false;

extern "C" DLL_IMPORT_EXPORT void MDXX_py_init() {
	if (!python_has_been_initialized) {
		Py_Initialize();
		std::string py_plugin_path_load = "import sys;import os;sys.path.append(r'";
		py_plugin_path_load += mdxx::main_program_dir;
		py_plugin_path_load += "plugins');";
		#ifndef WIN32
		py_plugin_path_load += "sys.path.append(r'/usr/lib/md++-git/plugins');";
		#endif
		// I should probably use Py_SetProgramName
		PyRun_SimpleString(py_plugin_path_load.c_str());
		python_has_been_initialized = true;
	}
}

void MDXX_py_finalize() {
	if (python_has_been_initialized) {
		if (Py_FinalizeEx() < 0) {
			MDXX_error(nullptr,
"Python had trouble closing for some reason. It shouldn't be a big deal\n"
"since the program is about to exit anyway.");
		}
	}
}

extern "C" DLL_IMPORT_EXPORT const char * MDXX_get_main_program_dir() {
	return mdxx::main_program_dir;
}
