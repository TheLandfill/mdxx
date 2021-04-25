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

#include "plugin_loader.h"
#include "compilation_info.h"
#include "clear_line.h"
#include "mdxx_get.h"
#include "thread_safe_print.h"
#include <memory>
#include <string>
#include <stdexcept>
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
		return LoadLibrary(dll_name);
	}
	#define LOAD_PLUGIN(X, Y) (import_func_ptr) GetProcAddress(X, Y);

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

	#define LOAD_PLUGIN(X, Y) get_plugin_import_func(X, Y);
	import_func_ptr get_plugin_import_func(MDXX_SHARED_HANDLE_TYPE handle, const char * function) {
		return (import_func_ptr)dlsym(handle, function);
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
	#pragma omp critical(first_plugin)
	{
		static bool first_plugin_loaded = true;
		if (first_plugin_loaded) {
			std::string abi_info;
			abi_info.reserve(512);
			abi_info += "ABI info below. If md++ has a problem, check to make sure compiler versions are compatible.\n\n";
			abi_info += MDXX_BOLD MDXX_VAR_COLOR "md++:\t\t";
			abi_info += MDXX_COMPILATION_INFO;
			abi_info += ".\n" MDXX_RESET;
			MDXX_print(stdout, abi_info.c_str());
			first_plugin_loaded = false;
		}
	}
	#pragma omp critical(load_plugin)
	{
		std::string full_library_name = plugin_dir + lib_prefix + shared_library_name + lib_suffix;
		import_func_ptr import_plugin_func = NULL;
		if (plugins.count(full_library_name) == 0) {
			auto plugin_handle = OPEN_SHARED_LIBRARY(full_library_name.c_str());
			if (plugin_handle != nullptr) {
				import_plugin_func = LOAD_PLUGIN(plugin_handle, "import_plugin");
				if (import_plugin_func == NULL) {
					plugin_load_error("import_plugin");
				}
			} else {
				std::string error_message = "Plugin ";
				error_message += shared_library_name;
				error_message += " does not exist.";
				throw std::runtime_error(error_message);
			}
			plugins[full_library_name] = Plugin_Info{plugin_handle, import_plugin_func};
			MDXX_print(stdout, MDXX_CONTEXT_COLOR);
			PRINT_COMPILATION_INFO(plugin_handle, "print_compilation_info");
			MDXX_print(stdout, MDXX_RESET);
		}
		plugins[full_library_name].import_plugin(this, mdxx_ptr);
	}
}

void Plugin_Loader::set_plugin_dir(const std::string& pd) {
#ifdef _MSC_FULL_VER
	static const char * plugin_directory = "plugins\\";
	if (pd.length() == 0 || pd.back() == '\\') {
		plugin_dir = pd;
	} else {
		plugin_dir = pd;
		plugin_dir += "\\";
	}
#else
	static const char * plugin_directory = "plugins/";
	plugin_dir = pd + "/";
#endif
	plugin_dir += plugin_directory;
}

std::string Plugin_Loader::get_plugin_dir() {
	return plugin_dir;
}

variable_map * Plugin_Loader::get_variable_map(void * id) {
	plugin_variable_maps.emplace(id, variable_map());
	return &plugin_variable_maps[id];
}

void Plugin_Loader::free_variable_map(void * id) {
	plugin_variable_maps.erase(id);
}

Plugin_Loader::~Plugin_Loader() {
	for (auto& library : plugins) {
		CLOSE_SHARED_LIBRARY(library.second.plugin_handle);
	}
}

template<>
const char * Expansion<Plugin_Loader*>::to_string() {
	std::stringstream strstr;
	strstr << "Plugin_Loader @ " << MDXX_GET(Plugin_Loader*, this) << MDXX_FUNC_COLOR << " (object)" << MDXX_RESET;
	data->plugin_loader_obj_id = strstr.str();
	return data->plugin_loader_obj_id.c_str();
}

}
