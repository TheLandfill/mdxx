#include "plugin_loader.h"
#include "compilation_info.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace mdxx {

void plugin_load_error(std::string function) {
	std::string error_message = "Plugin needs to implement ";
	error_message += function;
	throw std::runtime_error(error_message);
}

Plugin_Loader::Plugin_Loader() {
	plugin_variable_maps.reserve(8191);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#include <libloaderapi.h>
	typedef void (__cdecl *MYPROC)();
	#define OPEN_SHARED_LIBRARY(X) open_dll(X)
	HINSTANCE open_dll(const char * dll_name) {
		HINSTANCE hinstlib = LoadLibrary(dll_name);
		if (hinstlib != NULL) {
			return hinstlib;
		} else {
			std::cerr << "Library " << dll_name << " was unable to load.";
			throw std::runtime_error("");
		}
	}
	typedef void (__cdecl *IMPORT_PROC)(Plugin_Loader *, MDXX_Manager *);
	#define LOAD_PLUGIN(X, Y) IMPORT_PROC import_plugin = (IMPORT_PROC) GetProcAddress(X, Y);\
	if (import_plugin != NULL) {\
		(import_plugin)(this, mdxx_ptr);\
	} else {\
		plugin_load_error(Y);\
	}

	#define PRINT_COMPILATION_INFO(X, Y) MYPROC print_compilation_info = (MYPROC) GetProcAddress(X, Y);\
	if (print_compilation_info != NULL) {\
		(print_compilation_info)();\
	} else {\
		plugin_load_error(Y);\
	}

	#define CLOSE_SHARED_LIBRARY(X) FreeLibrary(X)

	#define LIB_PREFIX ""
	#define LIB_SUFFIX ".dll"
#else
	#define OPEN_SHARED_LIBRARY(X) dlopen(X, RTLD_LAZY)

	#define LOAD_PLUGIN(X, Y) void(*import_plugin)(mdxx::Plugin_Loader *, mdxx::MDXX_Manager *);\
	*(void **)(&import_plugin) = dlsym(X, Y);\
	if (import_plugin != NULL) {\
		(*import_plugin)(this, mdxx_ptr);\
	} else {\
		plugin_load_error(Y);\
	}

	#define PRINT_COMPILATION_INFO(X, Y) void(*print_compilation_info)();\
	*(void **)(&print_compilation_info) = dlsym(X, Y);\
	if (print_compilation_info != NULL) {\
		(*print_compilation_info)();\
	} else {\
		plugin_load_error(Y);\
	}

	#define CLOSE_SHARED_LIBRARY(X) dlclose(X)

	#define LIB_PREFIX "lib"
	#define LIB_SUFFIX ".so"
#endif

static std::string lib_prefix = LIB_PREFIX;
static std::string lib_suffix = LIB_SUFFIX;

void Plugin_Loader::load_plugin(MDXX_Manager * mdxx_ptr, const char * shared_library_name) {
	static bool first_plugin_loaded = true;
	if (first_plugin_loaded) {
		std::cout << "ABI info below. If md++ has a problem, check to make sure compiler versions are compatible.\n\n";
		std::cout << "md++:\t\t" << MDXX_COMPILATION_INFO << ".\n";
		first_plugin_loaded = false;
	}
	std::string full_library_name = plugin_dir + lib_prefix + shared_library_name + lib_suffix;
	std::cout << "Attempting to load " << full_library_name << "." << std::flush;
	plugins.push_back(OPEN_SHARED_LIBRARY(full_library_name.c_str()));
	if (plugins.back() != nullptr) {
		LOAD_PLUGIN(plugins.back(), "import_plugin");
		PRINT_COMPILATION_INFO(plugins.back(), "print_compilation_info");
	} else {
		std::cerr << "\n";
		std::string error_message = "Plugin ";
		error_message += shared_library_name;
		error_message += " does not exist.";
		throw std::runtime_error(error_message);
	}
}

void Plugin_Loader::set_plugin_dir(const std::string& pd) {
#ifdef _MSC_FULL_VER
	if (pd.length() == 0 || pd.back() == '\\') {
		plugin_dir = pd;
	} else {
		plugin_dir = pd;
		plugin_dir += "\\";
	}
#else
	plugin_dir = pd + "/";
#endif
}

std::string Plugin_Loader::get_plugin_dir() {
	return plugin_dir;
}

variable_map * Plugin_Loader::get_variable_map(void * id) {
	plugin_variable_maps.insert({id, new variable_map()});
	return plugin_variable_maps[id];
}

void Plugin_Loader::free_variable_map(void * id) {
	delete plugin_variable_maps[id];
	plugin_variable_maps.erase(id);
}

Plugin_Loader::~Plugin_Loader() {
	for (auto library : plugins) {
		CLOSE_SHARED_LIBRARY(library);
	}
}

template<>
const char * Expansion<Plugin_Loader*>::to_string() {
	std::stringstream strstr;
	strstr << "<Plugin_Loader object @ " << *static_cast<Plugin_Loader**>(this->get_data()) << ">";
	data->plugin_loader_obj_id = strstr.str();
	return data->plugin_loader_obj_id.c_str();
}

}
