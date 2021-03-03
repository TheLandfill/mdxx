#ifndef MDXX_EXPANSION_H
#define MDXX_EXPANSION_H
#include "dll_info.h"
#include <string>
#include <sstream>
#include <vector>
#include <memory>

namespace mdxx {

#define MDXX_EXP(X, Y) new mdxx::Expansion<X>(Y)

struct Expansion_Base {
	virtual void * get_data() = 0;
	virtual const char * to_string() = 0;
	virtual Expansion_Base* make_deep_copy() = 0;
};

typedef char * (*gen_func)(Expansion_Base** args, size_t argc);

template<typename T>
struct Expansion : Expansion_Base {
	T data;
	std::string to_str;
	std::string type;
	Expansion(T d) : data(d), type(typeid(T).name()) {}
	void * get_data() override {
		return &data;
	}
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	~Expansion() {}
};

template<>
struct Expansion<std::string> : Expansion_Base {
	std::string data;
	Expansion(std::string d);
	void * get_data() override;
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
};

template <typename T>
struct Expansion<T *> : Expansion_Base {
	T * data;
	std::string to_str;
	std::string type;
	Expansion(T * d) : data(d), type(typeid(T).name()) {}
	void * get_data() override {
		return &data;
	}
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
	~Expansion() {}
};

template<>
struct Expansion<gen_func> : Expansion_Base {
	gen_func func;
	std::string name;
	std::string full_name;
	Expansion(gen_func function, std::string name);
	void * get_data() override;
	const char * to_string() override;
	Expansion_Base* make_deep_copy() override;
};

template<typename T>
const char * Expansion<T>::to_string() {
	to_str = std::string(data);
	return to_str.c_str();
}

template<typename T>
const char * Expansion<T*>::to_string() {
	std::stringstream strstr;
	strstr << std::string(*data) << " (Pointer) @ " << data;
	to_str = strstr.str();
	return to_str.c_str();
}

template<>
const char * Expansion<char *>::to_string();

template<>
const char * Expansion<const char *>::to_string();

template<typename T>
Expansion_Base* Expansion<T>::make_deep_copy() {
	return new Expansion<T>(data);
}

template<typename T>
Expansion_Base* Expansion<T *>::make_deep_copy() {
	return new Expansion<T*>(data);
}

}

#endif
