#include "thread_safe_print.h"
#include "mdxx_manager.h"
#include <string>
#include <cstring>

void MDXX_error(mdxx::MDXX_Manager* md, const char * str) {
	std::string output;
	output.reserve(strlen(str) + 128);
	output += "\n";
	output += MDXX_ERROR_PREFIX;
	output += str;
	output += "\n";
	output += MDXX_RESET;
	MDXX_thread_safe_print(stderr, output.c_str());
	MDXX_print_current_line_and_exit(md);
}

void MDXX_warn(const char * str) {
	std::string output;
	output.reserve(strlen(str) + 128);
	output += "\n";
	output += MDXX_WARNING_PREFIX;
	output += str;
	output += MDXX_RESET;
	output += "\n";
	MDXX_thread_safe_print(stderr, output.c_str());
}

void MDXX_thread_safe_print(FILE* out, const char * str) {
	#pragma omp critical(thread_safe_printing)
	{
		fprintf(out, "%s", str);
	}
}
