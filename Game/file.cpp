#include "file.hpp"
#include <fstream>
#include <sstream>

namespace file {

	std::string read_all(const char* filename) {
		std::ifstream in(filename);
		std::ostringstream s;
		if(in.is_open()) {
			s << in.rdbuf();
		}
		else
			printf("Couldn't open file '%s' !", filename);
		in.close();
		return s.str();
	}

	array::array read_binary(const char* filename) {
		int file_size = 0;
		array::array bytes = array::create(sizeof(u8), file_size);
		//TODO
		return bytes;
	}

}