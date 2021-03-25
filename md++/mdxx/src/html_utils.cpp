#include "html_utils.h"
#include "expansion.h"
#include "join.h"
#include "c_string_copy.h"
#include "sanitize_user_input.h"
#include "mdxx_get.h"
#include "html_manager.h"
#include "mdxx_manager.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include <iostream>
#include <unordered_map>

namespace mdxx {

const static bool valid_url_chars[256] = {
	/* NUL		*/	false,
	/* SOH		*/	false,
	/* STX		*/	false,
	/* ETX		*/	false,
	/* EOT		*/	false,
	/* ENQ		*/	false,
	/* ACK		*/	false,
	/* BEL		*/	false,
	/* BS		*/	false,
	/* HT		*/	false,
	/* LF		*/	false,
	/* VT		*/	false,
	/* FF		*/	false,
	/* CR		*/	false,
	/* SO		*/	false,
	/* SI		*/	false,
	/* DLE		*/	false,
	/* DC1		*/	false,
	/* DC2		*/	false,
	/* DC3		*/	false,
	/* DC4		*/	false,
	/* NAK		*/	false,
	/* SYN		*/	false,
	/* ETB		*/	false,
	/* CAN		*/	false,
	/* EM		*/	false,
	/* SUB		*/	false,
	/* ESC		*/	false,
	/* FS		*/	false,
	/* GS		*/	false,
	/* RS		*/	false,
	/* US		*/	false,
	/* space	*/	false,
	/* !		*/	true,
	/* "		*/	false,
	/* #		*/	true,
	/* $		*/	true,
	/* %		*/	true,
	/* &		*/	true,
	/* '		*/	true,
	/* (		*/	true,
	/* )		*/	true,
	/* *		*/	true,
	/* +		*/	true,
	/* ,		*/	true,
	/* -		*/	true,
	/* .		*/	true,
	/* /		*/	true,
	/* 0		*/	true,
	/* 1		*/	true,
	/* 2		*/	true,
	/* 3		*/	true,
	/* 4		*/	true,
	/* 5		*/	true,
	/* 6		*/	true,
	/* 7		*/	true,
	/* 8		*/	true,
	/* 9		*/	true,
	/* :		*/	true,
	/* ;		*/	true,
	/* <		*/	false,
	/* =		*/	true,
	/* >		*/	false,
	/* ?		*/	true,
	/* @		*/	true,
	/* A		*/	true,
	/* B		*/	true,
	/* C		*/	true,
	/* D		*/	true,
	/* E		*/	true,
	/* F		*/	true,
	/* G		*/	true,
	/* H		*/	true,
	/* I		*/	true,
	/* J		*/	true,
	/* K		*/	true,
	/* L		*/	true,
	/* M		*/	true,
	/* N		*/	true,
	/* O		*/	true,
	/* P		*/	true,
	/* Q		*/	true,
	/* R		*/	true,
	/* S		*/	true,
	/* T		*/	true,
	/* U		*/	true,
	/* V		*/	true,
	/* W		*/	true,
	/* X		*/	true,
	/* Y		*/	true,
	/* Z		*/	true,
	/* [		*/	true,
	/* \		*/	false,
	/* ]		*/	true,
	/* ^		*/	false,
	/* _		*/	true,
	/* `		*/	false,
	/* a		*/	true,
	/* b		*/	true,
	/* c		*/	true,
	/* d		*/	true,
	/* e		*/	true,
	/* f		*/	true,
	/* g		*/	true,
	/* h		*/	true,
	/* i		*/	true,
	/* j		*/	true,
	/* k		*/	true,
	/* l		*/	true,
	/* m		*/	true,
	/* n		*/	true,
	/* o		*/	true,
	/* p		*/	true,
	/* q		*/	true,
	/* r		*/	true,
	/* s		*/	true,
	/* t		*/	true,
	/* u		*/	true,
	/* v		*/	true,
	/* w		*/	true,
	/* x		*/	true,
	/* y		*/	true,
	/* z		*/	true,
	/* {		*/	false,
	/* |		*/	false,
	/* }		*/	false,
	/* ~		*/	true,
	/* DEL		*/	false,
// Extended ASCII / UTF-8
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false,
					false
// Lookup table ftw, lol, rofl
};

static const std::string attribute_prefix = "mdxx-";

bool Tag_Handler::catch_bad_urls(std::string url) {
	std::vector<std::string> valid_protocols{ "http://", "https://", "ftp://", "mailto:" };
	for (unsigned char c : url) {
		if (!valid_url_chars[c]) {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "ERROR: The url\n\n\t<";
			error_message += url;
			error_message += ">\n\nis using the illegal character `";
			error_message +=  c;
			error_message += "` (ASCII NUMBER ";
			error_message += std::to_string((unsigned int)c);
			error_message += ") and is therefore unsafe.\n";
			throw std::runtime_error(error_message);
		}
	}
	for (auto& protocol : valid_protocols) {
		if (protocol == url.substr(0, protocol.length())) {
			return true;
		}
	}
	for (char c : url) {
		if (c == '.' || c == '/') {
			break;
		}
		if (c == ':') {
			std::string error_message;
			error_message.reserve(1024);
			error_message += "ERROR: The url\n\n\t<";
			error_message += url;
			error_message += ">\n\nis using an invalid protocol. Valid protocols are\n";
			for (auto& protocol : valid_protocols) {
				error_message += "\t";
				error_message += protocol;
				error_message += "\n";
			}
			error_message += "\nIf this was in error (e.g. you had a port without a protocol like\n"
			"`example.com:443`), add a valid protocol to the start of the url\n";
			throw std::runtime_error(error_message);
		}
	}
	return false;
}

std::vector<std::string> Tag_Handler::organize_user_input() {
	std::vector<std::string> user_input;
	size_t arg_offset = required_url_attribute_list.size();
	user_input.reserve(argc - arg_offset);
	for (size_t i = arg_offset; i < argc; i++) {
		user_input.emplace_back(MDXX_GET(const char *, args[i]));
	}
	return user_input;
}

str_map Tag_Handler::create_attribute_map() {
	str_map attribute_map;
	attribute_map.reserve(attribute_list.size());
	for (auto& attribute : attribute_list) {
		attribute_map[attribute_prefix + attribute] = "";
	}
	return attribute_map;
}

void sort_user_input_into_attribute_map(str_map& attribute_map, const std::vector<std::string>& user_input, const std::string& initial_state) {
	std::string cur_word = attribute_prefix + initial_state;
	for (auto& word : user_input) {
		if (attribute_map.count(word) > 0) {
			cur_word = word;
			continue;
		}
		attribute_map[cur_word] += word;
		attribute_map[cur_word] += " ";
	}
	for (auto& pair : attribute_map) {
		if (pair.second.length() > 1) {
			pair.second.pop_back();
		}
	}
}

void check_and_throw_error_missing_urls(const char * tag, const std::vector<std::string>& required_url_attribute_list, const size_t argc) {
	if (argc < required_url_attribute_list.size()) {
		std::string error_message;
		error_message.reserve(1024);
		error_message += tag;
		error_message += " requires ";
		if (required_url_attribute_list.size() == 1) {
			error_message += "a";
		} else {
			error_message += required_url_attribute_list.size();
		}
		error_message += " url";
		if (required_url_attribute_list.size() > 1) {
			error_message += "s";
		}
		error_message += ", in the order\n";
		for (auto& url_attribute : required_url_attribute_list) {
			error_message += "\t";
			error_message += url_attribute;
			error_message += "\n";
		}
		error_message += "Other attributes can be added after the urls.";
		throw std::runtime_error(
			error_message
		);
	}
}

std::vector<std::string> Tag_Handler::get_url_attribute_value_list() {
	std::vector<std::string> url_attribute_value_list;
	url_attribute_value_list.reserve(required_url_attribute_list.size());
	check_and_throw_error_missing_urls(tag, required_url_attribute_list, argc);
	for (size_t i = 0; i < required_url_attribute_list.size(); i++) {
		const char * cur_url = MDXX_GET(const char *, args[i]);
		std::string actual_url;
		actual_url.reserve(10);
		if (!catch_bad_urls(cur_url)) {
			actual_url += "http://";
		}
		url_attribute_value_list.emplace_back(actual_url + cur_url);
	}
	return url_attribute_value_list;
}

Basic_String Tag_Handler::generate_tag() {
	std::vector<std::string> url_attribute_value_list = get_url_attribute_value_list();
	std::vector<std::string> user_input = organize_user_input();
	std::unordered_map<std::string, std::string> attributes = create_attribute_map();
	if (attribute_list.size() > 0) {
		sort_user_input_into_attribute_map(attributes, user_input, attribute_list[0]);
	}

	size_t user_input_length = join_length(user_input, " ");
	size_t input_urls_total_length = join_length(url_attribute_value_list, "");
	std::string output;
	const size_t non_user_input_extra_length = 100;// some extra as a buffer
	output.reserve((user_input_length + input_urls_total_length + non_user_input_extra_length) * 2); // More buffer
	output += "<";
	output += tag;
	for (size_t i = 0; i < required_url_attribute_list.size(); i++) {
		output += " ";
		output += required_url_attribute_list[i];
		output += "=\"";
		output += url_attribute_value_list[i];
		output += "\"";
	}
	for (size_t i = (attribute_list.size() == 0 || attribute_list[0] == "text"); i < attribute_list.size(); i++) {
		std::string cur_attribute = attribute_prefix + attribute_list[i];
		if (attributes[cur_attribute] != "") {
			output += " ";
			output += attribute_list[i];
			output += "=\"";
			output += replace_double_quotes(attributes[cur_attribute]);
			output += "\"";
		}
	}
	output += ">";
	if (attributes["mdxx-text"] != "") {
		output += attributes["mdxx-text"];
		output += "</";
		output += tag;
		output += ">";
	}
	return {c_string_copy(output), output.length()};
}

// First argument is the HTML_Manager*, second could be the inline, then href, then src
char * img_link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	static const std::string inline_check = "inline";
	size_t args_offset = 2;
	if (argc > 2 && inline_check != MDXX_GET(const char *, args[1])) {
		HTML_Manager * html = MDXX_GET(HTML_Manager*, args[0]);
		html->check_and_close_paragraph();
		args_offset -= 1;
	}
	if (args_offset >= argc) {
		std::string error_message;
		error_message.reserve(1024);
		error_message += "ERROR: Not enough arguments provided to img-a. Arguments provided were\n\n";
		for (size_t i = 0; i < argc; i++) {
			error_message += "\t";
			error_message += args[i]->to_string();
			error_message += "\n";
		}
		std::cerr << error_message << std::flush;
		MDXX_print_current_line_and_exit(mdxx);
		return nullptr;
	}
	Tag_Handler tl{args + args_offset, 1, "a", {""}, {"href"}};
	Basic_String link_part = tl.generate_tag();
	args_offset += 1;
	Tag_Handler t{
		args + args_offset,
		argc - args_offset,
		"img",
		{"alt", "title", "width", "height", "id", "class"},
		{"src"}
	};
	Basic_String image_part = t.generate_tag();
	std::string output;
	output.reserve(2 * (link_part.length + image_part.length + 5));
	output += link_part.str;
	output += image_part.str;
	output += "</a>";
	return c_string_copy(output);
}

char * img(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	static const std::string inline_check = "inline";
	size_t args_offset = 2;
	if (argc > 2 && inline_check != MDXX_GET(const char *, args[1])) {
		HTML_Manager * html = MDXX_GET(HTML_Manager*, args[0]);
		html->check_and_close_paragraph();
		args_offset -= 1;
	}
	Tag_Handler t{
		args + args_offset,
		argc - args_offset,
		"img",
		{"alt", "title", "width", "height", "id", "class"},
		{"src"}
	};
	return t.generate_tag().str;
}

char * link(MDXX_Manager * mdxx, Expansion_Base** args, size_t argc) {
	(void)mdxx;
	Tag_Handler t{
		args,
		argc,
		"a",
		{"text", "title", "alt", "id", "class"},
		{"href"}
	};
	return t.generate_tag().str;
}

Tag_Handler::Tag_Handler(
	Expansion_Base** args_c,
	size_t argc_c,
	const char * tag_c,
	const std::vector<std::string>& a,
	const std::vector<std::string>& r
) :
	args(args_c),
	argc(argc_c),
	tag(tag_c),
	attribute_list(a),
	required_url_attribute_list(r)
{}

}
