#include "plugin_loader.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace mdxx {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define OPEN_SHARED_LIBRARY(X) LoadLibrary(TEXT(X))
	#define LOAD_FUNCTIONS(X, Y) (MYPROC) add_functions_to_dictionary = (MYPROC) GetProcAddress(X, Y);\
	(add_functions_to_dictionary)()

	#define CLOSE_SHARED_LIBRARY(X) FreeLibrary(X)

	#define LIB_PREFIX ""
	#define LIB_SUFFIX ".dll"
#else
	#define OPEN_SHARED_LIBRARY(X) dlopen(X, RTLD_LAZY)

	#define LOAD_FUNCTIONS(X, Y) void(*add_functions_to_dictionary)();\
	*(void **)(&add_functions_to_dictionary) = dlsym(X, Y);\
	(*add_functions_to_dictionary)()

	#define CLOSE_SHARED_LIBRARY(X) dlclose(X)

	#define LIB_PREFIX "lib"
	#define LIB_SUFFIX ".so"
#endif

static std::string lib_prefix = LIB_PREFIX;
static std::string lib_suffix = LIB_SUFFIX;

void Plugin_Loader::load_plugin(const char * shared_libary_name) {
	std::cout << "Attempting to load " << shared_libary_name << ".\n";
	std::string full_library_name = plugin_dir + lib_prefix + shared_libary_name + lib_suffix;
	std::cout << "Full name: " << full_library_name << "\n";
	plugins.push_back(OPEN_SHARED_LIBRARY(full_library_name.c_str()));
	if (plugins.back() != nullptr) {
		LOAD_FUNCTIONS(plugins.back(), "import_plugin");
	} else {
		std::string error_message = "Plugin ";
		error_message += shared_libary_name;
		error_message += " does not exist.";
		throw std::runtime_error(error_message);
	}
}

std::string Plugin_Loader::set_plugin_dir(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	std::string pd = *static_cast<std::string*>(args.at(0)->get_data());
	plugin_dir = pd + "/";
	return "";
}

void Plugin_Loader::set_plugin_dir(const std::string& pd) {
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

std::string Plugin_Loader::plugin_dir = "";

}
