#ifndef MDXX_SANITIZE_USER_INPUT_H
#define MDXX_SANITIZE_USER_INPUT_H
#include <string>

namespace mdxx {

std::string replace_angle_brackets(const std::string& str);

void remove_angle_brackets(std::string& str);

std::string replace_quotes(const std::string& str);

void remove_quotes(std::string& str);

// Replaces everything with the html entity version except ampersands
std::string replace_all_html_chars(const std::string& str);

// Removes everything but ampersands
void remove_all_html_chars(std::string& str);

void correctly_substitute_ampersands(std::string& str);

}
#endif
