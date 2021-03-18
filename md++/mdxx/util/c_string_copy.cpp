#include "c_string_copy.h"

namespace mdxx {

char * c_string_copy(const std::string& str) {
	char * output = new char[str.length() + 1];
	std::copy(str.begin(), str.end(), output);
	output[str.length()] = '\0';
	return output;
}

}
