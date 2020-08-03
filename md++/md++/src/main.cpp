#include <iostream>

void usage_message(char * program_name);

int main(int argc, char ** argv) {
	if (argc < 3) {
		usage_message(argv[0]);
		return 1;
	}
	std::cout << "Hello, World!\n";
	return 0;
}

void usage_message(char * program_name) {
	std::cerr << program_name << "\ttemplate_location\ttumd_file[s]\n";
}
