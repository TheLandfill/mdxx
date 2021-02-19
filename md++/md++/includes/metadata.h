#ifndef MDXX_METADATA_H
#define MDXX_METADATA_H
#include "mdxx_manager.h"
#include <vector>
#include <string>

namespace mdxx {

extern "C" char * MDXX_read_metadata_file(Expansion_Base** args, size_t argc);

}

#endif
