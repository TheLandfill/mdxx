#include "py_init.h"
#include "thread_safe_print.h"
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
		py_plugin_path_load += "' + 'plugins');";
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
			std::cerr << MDXX_ERROR_PREFIX <<
"Python had trouble closing for some reason. It shouldn't be a big deal\n"
"since the program is about to exit anyway."
				<< std::endl;
		}
	}
}

extern "C" DLL_IMPORT_EXPORT const char * MDXX_get_main_program_dir() {
	return mdxx::main_program_dir;
}
