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
	const char * main_program_dir;
}

void MDXX_py_finalize();
void usage_message(char * program_name);

extern "C" DLL_IMPORT_EXPORT int MDXX_run_program(int argc, char ** argv) {
	auto start_time = std::chrono::high_resolution_clock::now();
	using namespace mdxx;
	if (argc < 3) {
		usage_message(argv[0]);
		return 1;
	}
	std::string main_dir = fs::path(argv[0]).parent_path().string();
	if (!(main_dir == "" || main_dir == "./")) {
		main_dir += fs::path::preferred_separator;
		main_dir += fs::path::preferred_separator;
	}
	main_program_dir = main_dir.c_str();
	Plugin_Loader pl;
	pl.set_plugin_dir(main_dir);
	fs::path template_path(argv[1]);
	int finished_webpages = 0;
	#pragma omp parallel for schedule(dynamic)
	for (int current_file = 2; current_file < argc; current_file++) {
		fs::path infile(fs::absolute(argv[current_file]));
		std::ifstream in{infile};
		MDXX_Manager mdxx = MDXX_Manager(in);
		fs::path outfile = infile;
		outfile.replace_extension(".html");
		std::ofstream out{outfile};
		HTML_Manager html{out};
		std::shared_ptr<Content_Manager> content = std::make_shared<Content_Manager>(html, mdxx);
		std::string template_file = template_path.string() + mdxx.next_line_no_nl();
		Template_Manager template_reader(html, content, template_file);
		fs::path metafile = infile;
		metafile.replace_extension(".json");
		mdxx.add_variable_to_context("default", "metafile", c_string_copy(metafile.string().c_str()));
		mdxx.add_variable_to_context<Plugin_Loader*>("default", "plugin-obj", &pl);
		template_reader.process_template();
		#pragma omp critical
		{
			finished_webpages++;
			std::cout << MDXX_CLEAR_LINE << finished_webpages << "/" << argc - 2 << " finished." << std::flush;
		}
	}
	std::cout << std::endl;
	auto end_time = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count() * 1000.0;
	std::cout << "Generated " << argc - 2 << " webpages in " << total_time << " ms\n";
	std::cout << "Average time per webpage was " << total_time / (argc - 2) << " ms\n";
	MDXX_py_finalize();
	return 0;
}

void usage_message(char * program_name) {
	std::cerr << program_name << " template_location mdxx_file\n";
}
