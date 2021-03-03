#define MDXX_INTERFACE_DLL
#include "dll_info.h"
#include "expansion.h"
#include "nlohmann/json.hpp"
#include "mdxx_manager.h"
#include "metadata.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace mdxx;

void list_required_fields(const std::vector<std::string>& fields) {
	std::cerr << "Create the file and set the following fields:\n";
	for (const auto& field : fields) {
		std::cerr << "\t" << field << "\n";
	}
}

extern "C" char * MDXX_read_metadata_file(Expansion_Base** args, size_t argc) {
	if (argc < 3) {
		std::cerr <<
"read_metadata_file expects at least three arguments: the MDXX_Manager as an\n"
"Expansion<MDXX_Manager*>, the filename as an Expansion<char *>, and the\n"
"context as an Expansion<char *>. The rest of the arguments are the fields\n"
"you want to see in the json file, e.g. title, author, etc."
		<< std::endl;
		exit(EXIT_FAILURE);
	}
	MDXX_Manager * mdxx = *static_cast<MDXX_Manager**>(args[0]->get_data());
	std::string filename = std::string(*static_cast<const char **>(args[1]->get_data()));
	std::string context = std::string(*static_cast<const char **>(args[2]->get_data()));
	std::vector<std::string> fields;
	fields.reserve(argc);
	for (size_t i = 3; i < argc; i++) {
		// If emplace_back doesn't work, revert it to
		// fields.push_back(std::string(*static_cast<const char **>(args[i]->get_data())));
		fields.emplace_back(*static_cast<const char **>(args[i]->get_data()));
	}
	std::ifstream reader(filename);
	if (!reader.good()) {
		std::cerr << "Warning: Missing required metadata file \"" << filename << "\".\n";
		list_required_fields(fields);
		return nullptr;
	}
	json j;
	reader >> j;
	if (!j.is_object()) {
		std::cerr << "Error: File \"" << filename << "\" needs to store a single json object.\n";
		list_required_fields(fields);
		return nullptr;
	}
	for (const auto& field : fields) {
		if (j.contains(field)) {
			mdxx->add_variable_to_context<std::string>(context.c_str(), field.c_str(), j[field].get<std::string>());
		} else {
			std::cerr << "Warning: Field \"" << field << "\" is missing and will be filled with a default value in the output.\n";
		}
	}
	return nullptr;
}
