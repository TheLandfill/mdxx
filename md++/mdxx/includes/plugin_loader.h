#ifndef MDXX_PLUGIN_LOADER_H
#define MDXX_PLUGIN_LOADER_H
#include "expansion.h"
#include "variable_map_definition.h"
#include "dll_info.h"
#include "mdxx_manager.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace mdxx {

class Plugin_Loader;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#define MDXX_SHARED_HANDLE_TYPE HINSTANCE
	typedef void (__cdecl *import_func_ptr)(Plugin_Loader *, MDXX_Manager *);
#else
	#include <dlfcn.h>
	#define MDXX_SHARED_HANDLE_TYPE void *
	typedef void (*import_func_ptr)(Plugin_Loader *, MDXX_Manager *);
#endif

struct Plugin_Info {
	MDXX_SHARED_HANDLE_TYPE plugin_handle;
	import_func_ptr import_plugin;
};

class Plugin_Loader {
public:
	Plugin_Loader();
	void load_plugin(MDXX_Manager * mdxx, const char * shared_libary_name);
	void set_plugin_dir(const std::string& pd);
	std::string get_plugin_dir();
	variable_map * get_variable_map(void * id);
	void free_variable_map(void * id);
	~Plugin_Loader();
public:
	std::string plugin_loader_obj_id;
private:
	std::string plugin_dir;
	std::unordered_map<std::string, Plugin_Info> plugins;
	std::unordered_map<void *, variable_map> plugin_variable_maps;
};

template<>
const char * Expansion<Plugin_Loader*>::to_string();

extern "C" DLL_IMPORT_EXPORT char * MDXX_load_plugins(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

}
#endif
