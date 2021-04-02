#include "thread_safe_print.h"

void MDXX_thread_safe_print(FILE* out, const char * str) {
	#pragma omp critical(thread_safe_printing)
	{
		fprintf(out, "%s", str);
	}
}
