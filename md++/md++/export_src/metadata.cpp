#include "expansion.h"
#include "nlohmann/json.hpp"
#include "mdxx_manager.h"
#include "metadata.h"
#include <iostream>
#include <fstream>

namespace mdxx {

using json = nlohmann::json;

void list_required_fields(const std::vector<std::string>& fields) {
	std::cerr << "Create the file and set the following fields:\n";
	for (const auto& field : fields) {
		std::cerr << "\t" << field << "\n";
	}
}

char * MDXX_read_metadata_file(Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr << "read_metadata_file expects at least two arguments: the filename and the context, both as const char **'s" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string filename = std::string(*static_cast<const char **>(args[0]->get_data()));
	std::string context = std::string(*static_cast<const char **>(args[1]->get_data()));
	std::vector<std::string> fields;
	fields.reserve(argc);
	for (size_t i = 2; i < argc; i++) {
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
			MDXX_Manager::add_variable_to_context<std::string>(context.c_str(), field.c_str(), j[field].get<std::string>());
		} else {
			std::cerr << "Warning: Field \"" << field << "\" is missing and will be filled with a default value in the output.\n";
		}
	}
	return nullptr;
}

}
