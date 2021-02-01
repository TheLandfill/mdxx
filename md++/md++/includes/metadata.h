#ifndef MDXX_METADATA_H
#define MDXX_METADATA_H
#include "mdxx_manager.h"
#include <vector>
#include <string>

namespace mdxx {

std::string read_metadata_file(std::vector<std::unique_ptr<Expansion_Base>>& args);

}

#endif
