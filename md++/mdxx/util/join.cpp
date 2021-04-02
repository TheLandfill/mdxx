#include "join.h"
#include "thread_safe_print.h"
#include <vector>
#include <string>

namespace mdxx {

std::string join(const std::vector<std::string>& strings, std::string connector) {
	if (strings.size() == 0) {
		MDXX_warn("join was called with an empty vector of strings. Returning an empty string.");
		return "";
	}
	std::string out;
	size_t final_size = join_length(strings, connector) + 1;
	out.reserve(final_size);
	out += strings[0];
	for (size_t i = 1; i < strings.size(); i++) {
		out += connector;
		out += strings[i];
	}
	return out;
}

size_t join_length(const std::vector<std::string>& strings, std::string connector) {
	size_t final_size = 0;
	for (auto& str : strings) {
		final_size += str.length();
	}
	final_size += strings.size() * connector.length();
	final_size -= connector.length();
	return final_size;
}

}
