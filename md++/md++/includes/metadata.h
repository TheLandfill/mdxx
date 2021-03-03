#ifndef MDXX_METADATA_H
#define MDXX_METADATA_H
#include "mdxx_manager.h"
#include "dll_info.h"
#include <vector>
#include <string>

extern "C" DLL_IMPORT_EXPORT char * MDXX_read_metadata_file(mdxx::Expansion_Base** args, size_t argc);


#endif
