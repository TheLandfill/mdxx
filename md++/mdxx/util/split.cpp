#include "split.h"
#include <vector>
#include <string>

namespace mdxx {

std::vector<std::string> split(const std::string& s, const char& c) {
	std::string buff{""};
	buff.reserve(s.size());
	std::vector<std::string> v;
	
	for(auto n : s) {
		if(n != c) {
			buff+=n;
		} else if(n == c && buff != "") {
			v.push_back(buff);
			buff = "";
		}
	}
	if(buff != "") {
		v.push_back(buff);
	}
	return v;
}

}
