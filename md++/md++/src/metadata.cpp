#include "metadata.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <vector>
#include <iostream>
#include <filesystem>

namespace mdxx {

using json = nlohmann::json;

void list_required_fields(const std::vector<std::string*>& fields) {
	std::cerr << "Create the file and set the following fields:\n";
	for (const auto& field : fields) {
		std::cerr << "\t" << *field << "\n";
	}
}

std::string read_metadata_file(std::vector<std::unique_ptr<Expansion_Base>>& args) {
	std::string& filename = *static_cast<std::string*>(args.at(0)->get_data());
	std::string& context = *static_cast<std::string*>(args.at(1)->get_data());
	std::vector<std::string*> fields;
	fields.reserve(args.size());
	for (size_t i = 2; i < args.size(); i++) {
		fields.push_back(static_cast<std::string*>(args[i]->get_data()));
	}
	std::ifstream reader(filename);
	if (!reader.good()) {
		std::cerr << "Warning: Missing required metadata file \"" << filename << "\".\n";
		list_required_fields(fields);
		return "";
	}
	json j;
	reader >> j;
	if (!j.is_object()) {
		std::cerr << "Error: File \"" << filename << "\" needs to store a single json object.\n";
		list_required_fields(fields);
		return "";
	}
	for (const auto& field : fields) {
		if (j.contains(*field)) {
			MDXX_Manager::add_variable_to_context<std::string>(context.c_str(), field->c_str(), j[*field].get<std::string>());
		} else {
			std::cerr << "Warning: Field \"" << field << "\" is missing and will be filled with a default value in the output.\n";
		}
	}
	return "";
}

}