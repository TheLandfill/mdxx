#define MDXX_INTERFACE_DLL
#include "dll_info.h"
#include "plugin_loader.h"
#include "expansion.h"
#include <iostream>

namespace mdxx {

char * MDXX_set_plugin_dir(Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr <<
"MDXX_set_plugin_dir expects two arguments, an Expansion<Plugin_Loader*>* that\n"
"points to the main Plugin_Loader (should be {{plugin-obj}}) and an\n"
"Expansion<char *>, which should be the path to the plugin directory."
		<< std::endl;
		exit(EXIT_FAILURE);
	}
	Plugin_Loader * pl = *static_cast<Plugin_Loader**>(args[0]->get_data());
	std::string pd = static_cast<const char*>(args[1]->get_data());
	pl->set_plugin_dir(pd + "/");
	return nullptr;
}

char * MDXX_load_plugins(Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr <<
"MDXX_load_plugins expects at least two arguments with the rest of the\n"
"arguments being the plugins you want to load as Expansion<char *> or\n"
"Expansion<const char *>. The first argument is the Plugin_Loader ad an\n"
"Expansion<Plugin_Loader*>* (should be {{plugin-obj}}) and the second argument\n"
"should be the MDXX_Manager as an Expansion<MDXX_Manager*>* (should be\n"
"{{mdxx}})."
		<< std::endl;
		exit(EXIT_FAILURE);
	}
	Plugin_Loader * pl = *static_cast<Plugin_Loader**>(args[0]->get_data());
	MDXX_Manager * mdxx = *static_cast<MDXX_Manager**>(args[1]->get_data());
	for (size_t i = 2; i < argc; i++) {
		std::string plugin_name = *static_cast<const char**>(args[i]->get_data());
		pl->load_plugin(mdxx, plugin_name.c_str());
	}
	return nullptr;
}

}
