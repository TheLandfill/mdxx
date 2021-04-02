#include "c_string_copy.h"
#include "mdxx_manager.h"
#include "html_manager.h"
#include "content_manager.h"
#include "template_manager.h"
#include "plugin_loader.h"
#include "metadata.h"
#include "clear_line.h"
#include <fstream>
#include <iostream>
#include <memory>
#if WIN32
#include <Windows.h>
#define MDXX_GET_EXE_LOCATION unsigned long long num_bytes_in_file = GetModuleFileLocation(NULL, main_program_dir, main_program_dir_buff_size); \
if (num_bytes_in_file == 0) { \
	 std::cerr << "ERROR: Program could not be found, which is really weird since you're running it.\nI have no idea how this happened." << std::endl;\
}
#else
#include <unistd.h>
#define MDXX_GET_EXE_LOCATION \
try { \
	main_program_dir = new char[main_program_dir_buff_size + 1]; \
} catch (std::bad_alloc& e) { \
	std::cerr << "ERROR: Insufficient memory. You need at least " << main_program_dir_buff_size + 1 << " bytes of RAM for\nthe dependency resolution to work properly. Even if you fix that, I don't like\nyour odds of getting the full program running." << std::endl;\
	exit(EXIT_FAILURE); \
} \
long num_bytes_in_filename = readlink("/proc/self/exe", main_program_dir, main_program_dir_buff_size + 1); \
if (num_bytes_in_filename < 0) { \
	std::cerr << "ERROR: Program could not be found, which is really weird since you're running it.\nI have no idea how this happened." << std::endl;\
	exit(EXIT_FAILURE); \
} \
main_program_dir[num_bytes_in_filename] = '\0';
#endif
#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif
#include <chrono>

namespace mdxx {
	char * main_program_dir;
	size_t main_program_dir_buff_size = 8192;
}

void MDXX_py_finalize();
void usage_message(const char * program_name);

extern "C" DLL_IMPORT_EXPORT int MDXX_run_program(int argc, char ** argv) {
	auto start_time = std::chrono::high_resolution_clock::now();
	using namespace mdxx;
	if (argc < 3) {
		usage_message("md++");
		return 1;
	}
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
		fs::path infile(fs::absolute(argv[current_file]));
		std::ifstream in{infile};
		MDXX_Manager mdxx = MDXX_Manager(in);
		fs::path outfile = infile;
		outfile.replace_extension(".html");
		HTML_Manager html{outfile.string()};
		std::shared_ptr<Content_Manager> content = std::make_shared<Content_Manager>(html, mdxx, infile.string());
		std::string template_file = template_path.string() + mdxx.next_line_no_nl();
		Template_Manager template_reader(html, content, template_file);
		fs::path metafile = infile;
		metafile.replace_extension(".json");
		mdxx.add_variable_to_context("default", "metafile", c_string_copy(metafile.string().c_str()));
		mdxx.add_variable_to_context<Plugin_Loader*>("default", "plugin-obj", &pl);
		template_reader.process_template();
		#pragma omp critical
		{
			finished_webpages += (!mdxx.had_error() && !template_reader.had_error());
			std::cout << MDXX_CLEAR_LINE << finished_webpages << "/" << argc - 2 << " finished." << std::flush << MDXX_CLEAR_LINE;
		}
	}
	std::cout << std::endl;
	auto end_time = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count() * 1000.0;
	std::cout << "Generated " << finished_webpages << " webpages in " << total_time << " ms\n";
	std::cout << "Average time per webpage was " << total_time / (argc - 2) << " ms\n";
	MDXX_py_finalize();
	delete[] main_program_dir;
	return 0;
}

void usage_message(const char * program_name) {
	std::cerr << program_name << " template_location mdxx_file[s]\n";
}
