#include "plugin_loader.h"
#include "compilation_info.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace mdxx {

void plugin_load_error(std::string function) {
	std::string error_message = "Plugin needs to implement ";
	error_message += function;
	throw std::runtime_error(error_message);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#include <libloaderapi.h>
	typedef void (__cdecl *MYPROC)();
	#define OPEN_SHARED_LIBRARY(X) open_dll(X)
	HINSTANCE open_dll(const char * dll_name) {
		size_t num_bytes = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, dll_name, -1, nullptr, 0);
		std::unique_ptr<LPWSTR[]> local_buffer = std::make_unique<LPWSTR[]>(num_bytes);
		if (MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, dll_name, -1, *(local_buffer.get()), num_bytes) == 0) {
			std::string error_message;
			switch (GetLastError()) {
			case ERROR_INSUFFICIENT_BUFFER:
				error_message = "Provided buffer size for converting filenames is not large enough, which should not be possible because we query the size before allocating.\n";
				break;
			case ERROR_INVALID_FLAGS:
				error_message = "Invalid flags in converting filenames, which should not be possible because we're using the default flags.\n";
				break;
			case ERROR_NO_UNICODE_TRANSLATION:
				error_message = "Filename \"";
				error_message += dll_name;
				error_message += "\" has invalid unicode.\n";
				break;
			case ERROR_INVALID_PARAMETER:
				error_message = "Invalid parameter in converting filenames, which shouldn't be possible because all parameters should be valid.\n";
			}
			throw std::runtime_error(error_message);
		} else {
			return LoadPackagedLibrary(*(local_buffer.get()), 0);
		}
	}
	#define LOAD_PLUGIN(X, Y) MYPROC import_plugin = (MYPROC) GetProcAddress(X, Y);\
	if (import_plugin != NULL) {\
		(import_plugin)();\
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

	#define LOAD_PLUGIN(X, Y) void(*import_plugin)();\
	*(void **)(&import_plugin) = dlsym(X, Y);\
	if (import_plugin != NULL) {\
		(*import_plugin)();\
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

void Plugin_Loader::load_plugin(const char * shared_library_name) {
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

char * MDXX_set_plugin_dir(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "set_plugin_dir expects one argument, the directory where the plugins are located." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string pd = static_cast<const char*>(args[0]->get_data());
	Plugin_Loader::set_plugin_dir(pd + "/");
	return nullptr;
}

void Plugin_Loader::set_plugin_dir(const std::string& pd) {
	plugin_dir = pd + "/";
}

std::string Plugin_Loader::get_plugin_dir() {
	return plugin_dir;
}

variable_map * Plugin_Loader::get_variable_map() {
	return &(plugin_variable_maps.emplace_back());
}

Plugin_Loader::~Plugin_Loader() {
	for (auto library : plugins) {
		CLOSE_SHARED_LIBRARY(library);
	}
}

std::string Plugin_Loader::plugin_dir = "";

}
