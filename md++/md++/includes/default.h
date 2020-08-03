#ifndef MDXX_DEFAULT_H
#define MDXX_DEFAULT_H
#include "expansion.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace mdxx {

extern std::string empty_str;

extern std::unordered_map<std::string, std::unique_ptr<Expansion_Base>> default_dict;

}

#endif
