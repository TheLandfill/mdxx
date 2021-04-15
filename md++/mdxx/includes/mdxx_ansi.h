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

#ifndef MDXX_ANSI_H
#define MDXX_ANSI_H

#define MDXX_VAR_COLOR "\x1b[38;2;128;255;200m"
#define MDXX_VAL_COLOR "\x1b[38;2;255;153;255m"
#define MDXX_BOLD "\x1b[1m"
#define MDXX_RESET "\x1b[0m\x1b[38;2;255;38;92m"
#define MDXX_RESET_COLOR "\x1b[39m\x1b[38;2;255;38;92m"
#define MDXX_RESET_FORMATTING MDXX_RESET;
#define MDXX_CONTEXT_COLOR "\x1b[1m\x1b[38;2;159;135;255m"
#define MDXX_LINE_COLOR "\x1b[1m\x1b[38;2;254;217;255m"
#define MDXX_FILE_COLOR "\x1b[38;2;170;196;255m"
#define MDXX_FUNC_COLOR "\x1b[38;2;175;234;255m"
#define MDXX_ERROR_COLOR "\x1b[38;2;255;121;0m"
#define MDXX_ERROR_PREFIX "\x1b[1m\x1b[4mERROR:"
#define MDXX_WARNING_COLOR "\x1b[38;2;255;250;205m"
#define MDXX_WARNING_PREFIX "\x1b[1m\x1b[4mWARNING:"
#define MDXX_CLEAR_LINE "\r\x1b[K"

#endif
