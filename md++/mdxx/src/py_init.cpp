#include "py_init.h"
#include <Python.h>
#include <iostream>
#include <filesystem>

static bool python_has_been_initialized = false;

extern "C" DLL_IMPORT_EXPORT void MDXX_py_init() {
	if (!python_has_been_initialized) {
		Py_Initialize();
		std::string py_plugin_path_load = "import sys;import os;sys.path.append(r'";
		py_plugin_path_load += mdxx::main_program_dir;
		py_plugin_path_load += "' + 'plugins');";
		// I should probably use Py_SetProgramName
		PyRun_SimpleString(py_plugin_path_load.c_str());
		python_has_been_initialized = true;
	}
}

void MDXX_py_finalize() {
	if (python_has_been_initialized) {
		if (Py_FinalizeEx() < 0) {
			std::cerr <<
				"ERROR: Python had trouble closing for some reason.\n"
				<< "It shouldn't be a big deal since the program is about to exit anyway."
				<< std::endl;
		}
	}
}
