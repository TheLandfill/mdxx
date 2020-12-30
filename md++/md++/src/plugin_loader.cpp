#include "plugin_loader.h"
#include <string>

namespace mdxx {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define OPEN_SHARED_LIBRARY(X) LoadLibrary(TEXT(X))
	#define LOAD_FUNCTIONS(X, Y) (MYPROC) add_functions_to_dictionary = (MYPROC) GetProcAddress(X, Y);\
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
	LOAD_FUNCTIONS(plugins.back(), "import_plugin");
}

void Plugin_Loader::set_plugin_dir(std::string pd) {
	plugin_dir = pd + "/";
}

std::string Plugin_Loader::get_plugin_dir() {
	return plugin_dir;
}

Plugin_Loader::~Plugin_Loader() {
	for (auto library : plugins) {
		CLOSE_SHARED_LIBRARY(library);
	}
}

}
