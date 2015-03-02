#include <string>
#include "array.hpp"

namespace file {

	std::string read_all(const char* filename);
	array::array read_binary(const char* filename);

}