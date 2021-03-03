#ifndef MDXX_VARIABLE_MAP_DEFINITION_H
#define MDXX_VARIABLE_MAP_DEFINITION_H
#include "expansion.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace mdxx {
	typedef std::unordered_map<std::string, std::unique_ptr<mdxx::Expansion_Base>> variable_map;
}

#endif
