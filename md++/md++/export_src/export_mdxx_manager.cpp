#include "mdxx_manager.h"
#include "context.h"
#include <memory>

extern "C" {

void MDXX_add_new_context(mdxx::MDXX_Manager * mdxx, const char * name, mdxx::Context * context) {
	std::unique_ptr<mdxx::Context> test(context);
	mdxx->add_raw_context(name, test);
}

}
