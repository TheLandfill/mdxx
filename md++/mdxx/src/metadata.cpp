#include "dll_info.h"
#include "expansion.h"
#include "nlohmann/json.hpp"
#include "mdxx_manager.h"
#include "metadata.h"
#include "sanitize_user_input.h"
#include "mdxx_get.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using json = nlohmann::json;
using namespace mdxx;

static void nice_print(const std::string& str, bool add_comma=true) {
	std::string output;
	output.reserve(256);
	output += "\"";
	output += str;
	output += "\"";
	std::cerr << "\t" << std::left << std::setw(20) << output << ": \"What you want " << str << " to be.\"";
	if (add_comma) {
		std::cerr << ",";
	}
	std::cerr << "\n";
}

void list_required_fields(const std::vector<std::string>& fields) {
	std::cerr << "The contents of the file should look like:\n\n";
	std::cerr << "{\n";
	for (size_t i = 0; i < fields.size() - 1; i++) {
		nice_print(fields[i]);
	}
	nice_print(fields.back(), false);
	std::cerr << "}\n\n";
}

extern "C" char * MDXX_read_metadata_file(mdxx::MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		std::cerr <<
"read_metadata_file expects at least two arguments: the filename as text and the\n"
"context as text. The rest of the arguments are the fields you want to see in\n"
"the json file, e.g. title, author, etc."
		<< std::endl;
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	std::string filename = std::string(MDXX_GET(const char *, args[0]));
	std::string context = std::string(MDXX_GET(const char *, args[1]));
	std::vector<std::string> fields;
	fields.reserve(argc);
	for (size_t i = 2; i < argc; i++) {
		// If emplace_back doesn't work, revert it to
		// fields.push_back(std::string(*static_cast<const char **>(args[i]->get_data())));
		fields.emplace_back(MDXX_GET(const char *, args[i]));
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
			std::string value = j[field].get<std::string>();
			remove_angle_brackets(value);
			mdxx->add_variable_to_context(context.c_str(), field.c_str(), mdxx::c_string_copy(replace_double_quotes(value)));
		} else {
			std::cerr << "Warning: Field \"" << field << "\" is missing and will be filled with a default value in the output.\n";
		}
	}
	return nullptr;
}
