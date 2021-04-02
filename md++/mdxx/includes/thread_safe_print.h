#ifndef MDXX_THREAD_SAFE_PRINT_H
#define MDXX_THREAD_SAFE_PRINT_H
#include "clear_line.h"
#include <cstdio>

namespace mdxx {
	class MDXX_Manager;
}

void MDXX_thread_safe_print(FILE* out, const char * str);
void MDXX_error(mdxx::MDXX_Manager* md, const char * str);
void MDXX_warn(const char * str);

#define MDXX_ERROR_PREFIX MDXX_CLEAR_LINE MDXX_ESC "[1;4;48;255;121;0m" "ERROR: " MDXX_ESC "[m"
#define MDXX_WARNING_PREFIX MDXX_CLEAR_LINE MDXX_ESC "[1;4;48;255;250;205m" "WARNING: " MDXX_ESC "[m"

#endif
