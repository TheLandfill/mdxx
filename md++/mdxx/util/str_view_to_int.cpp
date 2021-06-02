#include "str_view_to_int.h"

namespace mdxx {

bool is_int(const std::string_view& input) {
	return is_integral_value<int>(input);
}

}
