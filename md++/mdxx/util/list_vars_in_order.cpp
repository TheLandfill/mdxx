#include "context.h"
#include <algorithm>

const char * MDXX_list_vars(mdxx::variable_map* variables, std::string& all_vars_as_text) {
	typedef std::pair<std::string, mdxx::Expansion_Base*> var_map_item;
	std::vector<var_map_item> vars_in_order;
	vars_in_order.reserve(variables->size());
	for (auto& vars_in_context : *variables) {
		vars_in_order.emplace_back(vars_in_context.first, vars_in_context.second.get());
	}
	std::sort(vars_in_order.begin(), vars_in_order.end(), [](const var_map_item& a, const var_map_item& b){ return a.first < b.first; });
	all_vars_as_text.clear();
	all_vars_as_text += MDXX_BOLD;
	all_vars_as_text += MDXX_VAR_COLOR;
	all_vars_as_text += "           Variable            ";
	all_vars_as_text += MDXX_RESET;
	all_vars_as_text += "┃                        ";
	all_vars_as_text += MDXX_BOLD;
	all_vars_as_text += MDXX_VAL_COLOR;
	all_vars_as_text += "Value\n";
	all_vars_as_text += MDXX_RESET;
	all_vars_as_text += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	for (auto& vars_in_context : vars_in_order) {
		all_vars_as_text += "    ";
		all_vars_as_text += MDXX_VAR_COLOR;
		all_vars_as_text += vars_in_context.first.substr(0, 22);
		if (vars_in_context.first.length() > 22) {
			all_vars_as_text += "...";
		}
		all_vars_as_text += MDXX_RESET;
		all_vars_as_text += std::string(25 - vars_in_context.first.length(), ' ');
		all_vars_as_text += "  ┃  ";
		all_vars_as_text += MDXX_VAL_COLOR;
		all_vars_as_text += "\"";
		all_vars_as_text += vars_in_context.second->to_string();
		all_vars_as_text += "\"";
		all_vars_as_text += MDXX_RESET;
		all_vars_as_text += "\n";
	}
	return all_vars_as_text.c_str();
}
