#ifndef MDXX_PLUGIN_LOADER_H
#define MDXX_PLUGIN_LOADER_H
#include <vector>
#include <string>

namespace mdxx {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#define MDXX_SHARED_HANDLE_TYPE HINSTANCE
#else
	#include <dlfcn.h>
	#define MDXX_SHARED_HANDLE_TYPE void *
#endif

class Plugin_Loader {
public:
	// Should have a shared library with functions/contexts and should use either
	// 		template<typename T>
	// 		void MDXX_Manager::add_new_context(const std::string name)
	// or
	// 		template<typename T>
	// 		void MDXX_Manager::add_new_context(const std::string name, variable_map variables)
	// to add a new context and
	// 		void MDXX_Manager::add_function(const std::string name, gen_func function)
	// to add a new function to a shared dictionary (i.e. unordered_map) that
	// all MDXX_Managers can use. Use the first one if you want to create an
	// empty context and add variables later or the second one if you already
	// have the variables ready to use.
	void load_plugin(const char * shared_libary_name);
	void set_plugin_dir(std::string plugin_dir);
	std::string get_plugin_dir();
	~Plugin_Loader();
private:
	std::string plugin_dir = "";
	std::vector<MDXX_SHARED_HANDLE_TYPE> plugins;
};

}
#endif
