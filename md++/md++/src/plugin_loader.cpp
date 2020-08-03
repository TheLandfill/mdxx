#include "plugin_loader.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define OPEN_SHARED_LIBRARY(X) LoadLibrary(TEXT(X))
	#define LOAD_FUNCTIONS(X) (MYPROC) add_functions_to_dictionary = (MYPROC) GetProcAddress(X)\
	(add_functions_to_dictionary)()
	#define CLOSE_SHARED_LIBRARY(X) FreeLibrary(X)
#else
	#define OPEN_SHARED_LIBRARY(X) dlopen(X, RTLD_LAZY)
	#define LOAD_FUNCTIONS(X, Y) void(*add_functions_to_dictionary)();\
	*(void **)(&add_functions_to_dictionary) = dlsym(X, Y);\
	(*add_functions_to_dictionary)()
	#define CLOSE_SHARED_LIBRARY(X) dlclose(X)
#endif


void Plugin_Loader::load_plugin(const char * shared_libary_name) {
	plugins.push_back(OPEN_SHARED_LIBRARY(shared_libary_name));
	LOAD_FUNCTIONS(plugins.back(), "add_functions");
}

Plugin_Loader::~Plugin_Loader() {
	for (auto library : plugins) {
		CLOSE_SHARED_LIBRARY(library);
	}
}
