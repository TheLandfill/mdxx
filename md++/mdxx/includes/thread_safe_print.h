#ifndef MDXX_THREAD_SAFE_PRINT_H
#define MDXX_THREAD_SAFE_PRINT_H
#include <cstdio>

namespace mdxx {
	class MDXX_Manager;
}

void MDXX_thread_safe_print(FILE* out, const char * str);
void MDXX_error(mdxx::MDXX_Manager* md, const char * str);
void MDXX_warn(const char * str);

#endif
