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

#if defined(_MSC_FULL_VER)
	#if defined(MDXX_EXTERNAL_CONTEXT) || defined(MDXX_INTERFACE_DLL)
		#define DLL_IMPORT_EXPORT __declspec(dllexport)
	#else
		#define DLL_IMPORT_EXPORT __declspec(dllimport)
	#endif
#else
	#define DLL_IMPORT_EXPORT
#endif

#define MDXX_STRX(X) #X
#define MDXX_STR(X) MDXX_STRX(X)
