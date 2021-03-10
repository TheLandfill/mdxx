#include "py_init.h"
#include <Python.h>
#include <iostream>

static bool python_has_been_initialized = false;

extern "C" DLL_IMPORT_EXPORT void MDXX_py_init() {
	if (!python_has_been_initialized) {
		Py_Initialize();
		// I should probably use Py_SetProgramName
		PyRun_SimpleString("import sys;import os;sys.path.append(os.getcwd() + os.path.sep + 'out' + os.path.sep + 'plugins')");
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
