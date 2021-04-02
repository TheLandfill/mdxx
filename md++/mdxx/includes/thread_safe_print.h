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

#ifdef WIN32
#define MDXX_ERROR_PREFIX MDXX_CLEAR_LINE "\x1b[1;4;48;255;121;0mERROR:\x1b[m"
#define MDXX_WARNING_PREFIX MDXX_CLEAR_LINE "\x1b[1;4;48;255;250;205mWARNING:\x1b[m"
#else
#define MDXX_ERROR_PREFIX MDXX_CLEAR_LINE "\033[1;4;48;255;121;0mERROR:\033[m"
#define MDXX_WARNING_PREFIX MDXX_CLEAR_LINE "\033[1;4;48;255;250;205mWARNING:\033[m"
#endif

#endif
