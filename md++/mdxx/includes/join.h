#ifndef MDXX_JOIN_H
#define MDXX_JOIN_H
#include <vector>
#include <string>

namespace mdxx {

std::string join(const std::vector<std::string>& strings, std::string connector);

size_t join_length(const std::vector<std::string>& strings, std::string connector);

}

#endif
