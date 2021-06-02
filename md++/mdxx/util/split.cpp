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

#include "split.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

namespace mdxx {

std::vector<std::string> split(const std::string& s, const char& c) {
	std::string buff{""};
	buff.reserve(s.size());
	std::vector<std::string> v;
	
	for(auto n : s) {
		if(n != c) {
			buff+=n;
		} else if(n == c && buff != "") {
			v.push_back(buff);
			buff.clear();
		}
	}
	if(buff != "") {
		v.push_back(buff);
	}
	return v;
}

std::vector<std::string_view> split(const std::string_view& s, const char& c) {
	std::vector<std::string_view> v;
	auto first = std::cbegin(s);
	while (first != std::cend(s)) {
		const auto second = std::find(first, std::cend(s), c);
		if (first != second) {
			v.emplace_back(s.substr(std::distance(s.cbegin(), first), std::distance(first, second)));
		}
		if (second == s.cend()) {
			break;
		}
		first = std::next(second);
	}
	
	return v;
}

std::vector<std::string_view> split(const char * s, const char& c) {
	return split(std::string_view{s}, c);
}

}
