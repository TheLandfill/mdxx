#ifndef MDXX_STR_VIEW_TO_INT_H
#define MDXX_STR_VIEW_TO_INT_H
#include <charconv>
#include <optional>
#include <string_view>

namespace mdxx {

template <typename T>
std::optional<T> to_integral_type(const std::string_view & input)
{
    T out;
    const std::from_chars_result result = std::from_chars(input.data(), input.data() + input.length(), out);
    if (
		result.ec == std::errc::invalid_argument
		|| result.ec == std::errc::result_out_of_range
		|| result.ptr != input.data() + input.length()
	)
    {
        return std::nullopt;
    }
    return out;
}

template <typename T>
bool is_integral_value(const std::string_view& input) {
	T out;
	const std::from_chars_result result = std::from_chars(input.data(), input.data() + input.length(), out);
	return !(result.ec == std::errc::invalid_argument
		|| result.ec == std::errc::result_out_of_range
		|| result.ptr != input.data() + input.length());
}

bool is_int(const std::string_view& input);
}
#endif
