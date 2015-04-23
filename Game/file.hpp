#include <string>
#include "array.hpp"

namespace file {

	//	reads all the content of a text file and returns as an std::string
	std::string read_all(const char* filename);
	//	reads a file as binary and returns the contents as an array (of bytes)
	array::array read_binary(const char* filename);

}