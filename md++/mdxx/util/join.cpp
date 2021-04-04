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

#include "join.h"
#include "thread_safe_print.h"
#include <vector>
#include <string>

namespace mdxx {

std::string join(const std::vector<std::string>& strings, std::string connector) {
	if (strings.size() == 0) {
		MDXX_warn("join was called with an empty vector of strings. Returning an empty string.");
		return "";
	}
	std::string out;
	size_t final_size = join_length(strings, connector) + 1;
	out.reserve(final_size);
	out += strings[0];
	for (size_t i = 1; i < strings.size(); i++) {
		out += connector;
		out += strings[i];
	}
	return out;
}

size_t join_length(const std::vector<std::string>& strings, std::string connector) {
	size_t final_size = 0;
	for (auto& str : strings) {
		final_size += str.length();
	}
	final_size += strings.size() * connector.length();
	final_size -= connector.length();
	return final_size;
}

}
