// md++: a fast, easy to use, and extensible static site generator.
// Copyright (C) 2021  Joseph Mellor
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "mdxx_manager.h"
#include "context.h"
#include "c_string_copy.h"
#include "thread_safe_print.h"
#include <memory>
#include <iostream>

extern "C" {

void MDXX_add_new_context(mdxx::MDXX_Manager * mdxx, const char * name, mdxx::Context * context) {
	std::unique_ptr<mdxx::Context> test(context);
	mdxx->add_raw_context(name, test);
}

char * MDXX_print_current_line(mdxx::MDXX_Manager * mdxx) {
	return mdxx::c_string_copy(mdxx->print_line());
}

void MDXX_print_current_line_and_exit(mdxx::MDXX_Manager * md) {
	std::string output;
	output.reserve(1024);
	output += "\nLine that caused the problem:\n";
	output += md->print_line();
	MDXX_print(stderr, output.c_str());
	md->error_exit();
}

void MDXX_add_general_variable_to_context(mdxx::MDXX_Manager * mdxx,
	const char * context_name,
	const char * variable,
	mdxx::Expansion_Base * value
) {
	mdxx->throw_exception_if_context_not_found(context_name);
	(*mdxx->get_context_dict())[context_name]->add_variable(variable, value);
}

void MDXX_add_function_variable_to_context(mdxx::MDXX_Manager * mdxx,
	const char * context_name,
	const char * variable,
	mdxx::gen_func func
) {
	mdxx->throw_exception_if_context_not_found(context_name);
	(*mdxx->get_context_dict())[context_name]->add_variable(variable, new mdxx::Expansion<mdxx::gen_func>(func, variable));
}

void MDXX_add_string_variable_to_context(mdxx::MDXX_Manager * mdxx,
	const char * context_name,
	const char * name,
	const char * value,
	bool immediate_expansion
) {
	mdxx->throw_exception_if_context_not_found(context_name);
	if (immediate_expansion) {
		std::string str_val = value;
		(*mdxx->get_context_dict())[context_name]->add_variable(name, new mdxx::Expansion<const char *>(mdxx::c_string_copy(mdxx->expand_line(str_val))));
	} else {
		(*mdxx->get_context_dict())[context_name]->add_variable(name, new mdxx::Expansion<const char *>(value));
	}
}


}
