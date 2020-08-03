#ifndef MDXX_EXPANSION_H
#define MDXX_EXPANSION_H
#include <string>
#include <vector>
#include <memory>

namespace mdxx {

struct Expansion_Base {
	virtual void * get_data();
	virtual std::string to_string();
	virtual std::unique_ptr<Expansion_Base> make_deep_copy();
};

typedef std::string (*gen_func)(std::vector<std::unique_ptr<Expansion_Base>>&);

template<typename T>
struct Expansion : Expansion_Base {
	T data;
	Expansion(T d) : data(d) {}
	void * get_data() override {
		return &data;
	}
	std::string to_string() override;
	std::unique_ptr<Expansion_Base> make_deep_copy() override;
};

template <typename T>
struct Expansion<T *> : Expansion_Base {
	T * data;
	Expansion(T * d) : data(d) {}
	void * get_data() override {
		return &data;
	}
	std::string to_string() override;
	std::unique_ptr<Expansion_Base> make_deep_copy() override;
};

template<>
struct Expansion<gen_func> : Expansion_Base {
	gen_func func;
	std::string name;
	Expansion<gen_func>(gen_func function, std::string n) : func(function), name(n) {}
	void * get_data() override {
		return &name;
	}
	std::string to_string() override;
	std::unique_ptr<Expansion_Base> make_deep_copy() override;
};

//template<typename T>
//std::unique_ptr<Expansion_Base>&& make_expansion(T data) {
//	return std::move(std::make_unique<Expansion_Base>(new Expansion<T>{data}));
//}

#define MAKE_EXPANSION(T, data) std::make_unique<Expansion<T>>(data)
#define MAKE_GEN_FUNC_EXPANSION(func) std::make_unique<Expansion<gen_func>>( func, #func )

template<typename T>
std::string Expansion<T>::to_string() {
	return static_cast<std::string>(data);
}

template<typename T>
std::unique_ptr<Expansion_Base> Expansion<T>::make_deep_copy() {
	return MAKE_EXPANSION(T, data);
}

template<typename T>
std::unique_ptr<Expansion_Base> Expansion<T *>::make_deep_copy() {
	return MAKE_EXPANSION(T*, data);
}

}

#endif
