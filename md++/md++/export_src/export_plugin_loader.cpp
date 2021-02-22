#define MDXX_INTERFACE_DLL
#include "dll_info.h"
#include "plugin_loader.h"
#include "expansion.h"
#include <iostream>

namespace mdxx {

char * MDXX_set_plugin_dir(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "set_plugin_dir expects two arguments, a pointer to the plugin_manager and the directory where the plugins are located." << std::endl;
		exit(EXIT_FAILURE);
	}
	Plugin_Loader * pl = *static_cast<Plugin_Loader**>(args[0]->get_data());
	std::string pd = static_cast<const char*>(args[1]->get_data());
	pl->set_plugin_dir(pd + "/");
	return nullptr;
}

}
