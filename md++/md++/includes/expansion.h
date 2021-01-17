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
	virtual ~Expansion_Base();
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
	Expansion(gen_func function, std::string n);
	void * get_data() override;
	std::string to_string() override;
	std::unique_ptr<Expansion_Base> make_deep_copy() override;
};

template<typename T>
std::string Expansion<T>::to_string() {
	return static_cast<std::string>(data);
}

template<typename T>
std::unique_ptr<Expansion_Base> Expansion<T>::make_deep_copy() {
	return std::make_unique<Expansion<T>>(data);
}

template<typename T>
std::unique_ptr<Expansion_Base> Expansion<T *>::make_deep_copy() {
	return std::make_unique<Expansion<T*>>(data);
}

}

#endif
