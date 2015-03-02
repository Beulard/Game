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
		long file_size = 0;
		FILE* f = NULL;
		fopen_s(&f, filename, "rb");
		fseek(f, 0, SEEK_END);
		file_size = ftell(f);
		array::array bytes = array::create(sizeof(u8), file_size);
		fseek(f, 0, SEEK_SET);
		fread((char*)bytes.data + array::HEADER_SIZE, sizeof(u8), file_size, f);
		return bytes;
	}

}