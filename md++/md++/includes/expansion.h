#ifndef MDXX_EXPANSION_H
#define MDXX_EXPANSION_H
#include <variant>
#include <string>
#include <vector>

typedef std::string (*gen_func)(std::vector<void *>&);

struct Expansion {
	std::string id;
	gen_func function;
	Expansion(std::string i, gen_func f = nullptr);
};

#endif
