#include "plugin_loader.h"
#include "expansion.h"
#include <iostream>

namespace mdxx {

char * MDXX_set_plugin_dir(Expansion_Base** args, size_t argc) {
	if (argc < 1) {
		std::cerr << "set_plugin_dir expects one argument, the directory where the plugins are located." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string pd = static_cast<const char*>(args[0]->get_data());
	Plugin_Loader::set_plugin_dir(pd + "/");
	return nullptr;
}

}
