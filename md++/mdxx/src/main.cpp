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

#include "c_string_copy.h"
#include "mdxx_manager.h"
#include "html_manager.h"
#include "content_manager.h"
#include "template_manager.h"
#include "plugin_loader.h"
#include "metadata.h"
#include "mdxx_ansi.h"
#include "thread_safe_print.h"
#include "gen_html.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#if WIN32
#include <Windows.h>
#define MDXX_GET_EXE_LOCATION \
try { \
	main_program_dir = new char[main_program_dir_buff_size + 1]; \
} catch (std::bad_alloc& e) { \
	std::cerr << MDXX_ERROR_PREFIX << "Insufficient memory. You need at least " << MDXX_FUNC_COLOR << main_program_dir_buff_size + 1 << MDXX_ERROR_COLOR << " bytes of RAM for\nthe dependency resolution to work properly. Even if you fix that, I don't like\nyour odds of getting the full program running." << MDXX_RESET << std::endl;\
	exit(EXIT_FAILURE); \
} \
unsigned long long num_bytes_in_filename = GetModuleFileName(NULL, main_program_dir, main_program_dir_buff_size + 1); \
if (num_bytes_in_filename == 0) { \
	std::cerr << MDXX_ERROR_PREFIX << "Program could not be found, which is really weird since you're running it.\nI have no idea how this happened." << MDXX_RESET << std::endl;\
	exit(EXIT_FAILURE); \
} \
if (num_bytes_in_filename == (main_program_dir_buff_size + 1)) { \
	std::cerr << MDXX_ERROR_PREFIX << "Program is in a path that has too many characters:\n\t" << MDXX_FILE_COLOR << main_program_dir << MDXX_RESET << "\n"; \
	exit(EXIT_FAILURE); \
}
#else
#include <unistd.h>
#define MDXX_GET_EXE_LOCATION \
try { \
	main_program_dir = new char[main_program_dir_buff_size + 1]; \
} catch (std::bad_alloc& e) { \
	std::cerr << MDXX_ERROR_PREFIX << "Insufficient memory. You need at least " << MDXX_FUNC_COLOR << main_program_dir_buff_size + 1 << MDXX_ERROR_COLOR << " bytes of RAM for\nthe dependency resolution to work properly. Even if you fix that, I don't like\nyour odds of getting the full program running." << MDXX_RESET << std::endl;\
	exit(EXIT_FAILURE); \
} \
long num_bytes_in_filename = readlink("/proc/self/exe", main_program_dir, main_program_dir_buff_size + 1); \
if (num_bytes_in_filename < 0) { \
	std::cerr << MDXX_ERROR_PREFIX << "Program could not be found, which is really weird since you're running it.\nI have no idea how this happened." << MDXX_RESET << std::endl;\
	exit(EXIT_FAILURE); \
} \
main_program_dir[num_bytes_in_filename] = '\0';
#endif
#include "filesys_wrapper.h"
#include <chrono>

namespace mdxx {
	char * main_program_dir;
	size_t main_program_dir_buff_size = 8192;
}

void MDXX_py_finalize();

static void print_summary(int finished_webpages, int num_attempted, double milliseconds) {
	std::string summary;
	summary.reserve(256);
	summary += "Successfully generated ";
	summary += std::to_string(finished_webpages);
	summary += " / ";
	summary += std::to_string(num_attempted);
	summary += " webpages in ";
	summary += std::to_string(milliseconds);
	summary += " ms\n"
		"Average time per webpage was ";
	summary += std::to_string(milliseconds / finished_webpages);
	summary += " ms\n"
		"\x1b[0m";
	MDXX_thread_safe_print(stdout, summary.c_str());
}

extern "C" DLL_IMPORT_EXPORT int MDXX_run_program(int argc, char ** argv) {
	auto start_time = std::chrono::high_resolution_clock::now();
	MDXX_thread_safe_print(stdout, MDXX_RESET);
	using namespace mdxx;
	MDXX_GET_EXE_LOCATION
	std::string main_dir = main_program_dir;
	delete[] main_program_dir;
	main_dir.erase(main_dir.find_last_of(fs::path::preferred_separator));
	main_dir += fs::path::preferred_separator;
	main_program_dir = c_string_copy(main_dir);
	Plugin_Loader pl;
	#ifdef WIN32
	pl.set_plugin_dir(main_dir);
	#else
	pl.set_plugin_dir("/usr/lib/md++-git");
	#endif
	fs::path template_path(argv[1]);
	int finished_webpages = 0;
	#pragma omp parallel for schedule(dynamic)
	for (int current_file = 2; current_file < argc; current_file++) {
		bool success = MDXX_gen_html(argv[current_file], template_path, &pl);
		#pragma omp critical(num_finished_webpages)
		finished_webpages += success;
	}
	MDXX_thread_safe_print(stdout, "\n");
	auto end_time = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count() * 1000.0;
	print_summary(finished_webpages, argc - 2, total_time);
	MDXX_py_finalize();
	delete[] main_program_dir;
	return 0;
}
