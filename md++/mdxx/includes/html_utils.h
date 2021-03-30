#ifndef MDXX_HTML_UTILS_H
#define MDXX_HTML_UTILS_H
#include "expansion.h"
#include <vector>
#include <unordered_map>

namespace mdxx {

char * link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

char * img_link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc);

typedef std::unordered_map<std::string, std::string> str_map;

struct Basic_String {
	char * str;
	size_t length;
};

class Tag_Handler {
public:
	Tag_Handler(
		Expansion_Base** args,
		size_t argc,
		const char * tag,
		const std::vector<std::string>& attribute_list,
		const std::vector<std::string>& required_url_attribute_list
	);
	Basic_String generate_tag(bool use_url_as_text = false);
private:
	bool catch_bad_urls(std::string url);
	std::vector<std::string> get_url_attribute_value_list();
	std::vector<std::string> organize_user_input();
	str_map create_attribute_map();
private:
	Expansion_Base** args;
	size_t argc;
	const char * tag;
	std::vector<std::string> attribute_list;
	std::vector<std::string> required_url_attribute_list;
};

}

#endif
