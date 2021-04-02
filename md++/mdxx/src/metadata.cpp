#include "dll_info.h"
#include "expansion.h"
#include "nlohmann/json.hpp"
#include "mdxx_manager.h"
#include "metadata.h"
#include "sanitize_user_input.h"
#include "mdxx_get.h"
#include "thread_safe_print.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;
using namespace mdxx;

static std::string nice_print(const std::string& str, bool add_comma=true) {
	std::string output;
	std::stringstream ss;
	output.reserve(256);
	output += "\"";
	output += str;
	output += "\"";
	ss << "\t" << std::left << std::setw(20) << output << ": \"What you want " << str << " to be.\"";
	if (add_comma) {
		ss << ",";
	}
	ss << "\n";
	return ss.str();
}

std::string list_required_fields(const std::vector<std::string>& fields) {
	std::stringstream ss;
	ss << "The contents of the file should look like:\n\n";
	ss << "{\n";
	for (size_t i = 0; i < fields.size() - 1; i++) {
		ss << nice_print(fields[i]);
	}
	ss << nice_print(fields.back(), false);
	ss << "}\n\n";
	return ss.str();
}

extern "C" char * MDXX_read_metadata_file(mdxx::MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	if (argc < 2) {
		MDXX_thread_safe_print(stderr,
			"read_metadata_file expects at least two arguments: the filename as text and the\n"
			"context as text. The rest of the arguments are the fields you want to see in\n"
			"the json file, e.g. title, author, etc.\n"
		);
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
		std::stringstream ss;
		ss << "Warning: Missing required metadata file \"" << filename << "\".\n";
		ss << list_required_fields(fields);
		MDXX_thread_safe_print(stderr, ss.str().c_str());
		return nullptr;
	}
	json j;
	reader >> j;
	if (!j.is_object()) {
		std::stringstream ss;
		ss << "Error: File \"" << filename << "\" needs to store a single json object.\n";
		ss << list_required_fields(fields);
		MDXX_thread_safe_print(stderr, ss.str().c_str());
		return nullptr;
	}
	for (const auto& field : fields) {
		if (j.contains(field)) {
			std::string value = j[field].get<std::string>();
			remove_angle_brackets(value);
			mdxx->add_variable_to_context(context.c_str(), field.c_str(), mdxx::c_string_copy(replace_double_quotes(value)));
		} else {
			std::stringstream ss;
			ss << "Warning: Field \"" << field << "\" is missing and will be filled with a default value in the output.\n";
			MDXX_thread_safe_print(stderr, ss.str().c_str());
		}
	}
	return nullptr;
}
