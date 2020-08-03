#include "content_manager.h"
#include <memory>
#include <sstream>

namespace mdxx {

MDXX_Manager& Content_Manager::get_mdxx() {
	return mdxx;
}

template<>
std::string Expansion<std::shared_ptr<Content_Manager>>::to_string() {
	std::stringstream output;
	output << "<Content_Manager @ " << this << ">";
	return output.str();
}

}
