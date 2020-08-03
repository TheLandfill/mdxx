#ifndef MDXX_PLUGIN_LOADER_H
#define MDXX_PLUGIN_LOADER_H
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#define MDXX_SHARED_HANDLE_TYPE HINSTANCE
#else
	#include <dlfcn.h>
	#define MDXX_SHARED_HANDLE_TYPE void *
#endif

class Plugin_Loader {
public:
	void load_plugin(const char * shared_libary_name);
	~Plugin_Loader();
private:
	std::vector<MDXX_SHARED_HANDLE_TYPE> plugins;
};

#endif
