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

#ifndef MDXX_SANITIZE_USER_INPUT_H
#define MDXX_SANITIZE_USER_INPUT_H
#include <string>

namespace mdxx {

std::string replace_angle_brackets(const std::string& str);

void remove_angle_brackets(std::string& str);

std::string replace_double_quotes(const std::string& str);

void remove_double_quotes(std::string& str);

std::string replace_quotes(const std::string& str);

void remove_quotes(std::string& str);

// Replaces everything with the html entity version except ampersands
std::string replace_all_html_chars(const std::string& str);

// Removes everything but ampersands
void remove_all_html_chars(std::string& str);

void correctly_substitute_ampersands(std::string& str);

}
#endif
