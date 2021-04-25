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

#include "mdxx_get.h"

namespace mdxx {

std::string MDXX_get_print_error_message(const char * received, const char * actual) {
	std::string error_message;
	error_message.reserve(2048);
	error_message += "Someone has passed in an argument of type\n\t`";
	error_message += received;
	error_message += "`\nwhen the expected type was\n\t`";
	error_message += actual;
	error_message += "`\n"
"Likely causes include a user redefining a builtin variable, a user redefining a\n"
"plugin variable, or errors in a plugin. If you can't read the types, displayed,\n"
"use the command\n\t`c++filt -t ";
	error_message += received;
	error_message += " ";
	error_message += actual;
	error_message += "`\nto see the unmangled typenames.";
	return error_message;
}

}
