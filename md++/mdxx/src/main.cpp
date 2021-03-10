#include "mdxx_manager.h"
#include "html_manager.h"
#include "content_manager.h"
#include "template_manager.h"
#include "plugin_loader.h"
#include "metadata.h"
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
#define PY_SSIZE_T_CLEAN
#include <Python.h>

void usage_message(char * program_name);

extern "C" DLL_IMPORT_EXPORT int MDXX_run_program(int argc, char ** argv) {
	auto start_time = std::chrono::high_resolution_clock::now();
	Py_Initialize();
	PyRun_SimpleString("import sys;import os;sys.path.append(os.getcwd() + os.path.sep + 'out' + os.path.sep + 'plugins')");
	using namespace mdxx;
	if (argc < 3) {
		usage_message(argv[0]);
		return 1;
	}
	std::string main_dir = fs::path(argv[0]).parent_path().string();
	Plugin_Loader pl;
	pl.set_plugin_dir(main_dir);
	fs::path template_path(argv[1]);
	//#pragma omp parallel for schedule(dynamic)
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
		mdxx.add_variable_to_context<std::string>("default", "metafile", metafile.string());
		mdxx.add_variable_to_context<Plugin_Loader*>("default", "plugin-obj", &pl);
		template_reader.process_template();
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count() * 1000.0;
	std::cout << "Generated " << argc - 2 << " webpages in " << total_time << " ms\n";
	std::cout << "Average time per webpage was " << total_time / (argc - 2) << " ms\n";
	if (Py_FinalizeEx() < 0) {
		std::cerr << "ERROR: Python had trouble closing for some reason.\n";
	}
	return 0;
}

void usage_message(char * program_name) {
	std::cerr << program_name << " template_location mdxx_file\n";
}
