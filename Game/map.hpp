#pragma once
#include "array.hpp"

//	stringmap describes a string indexed map where the data can be anything
//	and the keys must be 16 characters long C-strings
//	the data can also be accessed by simple indexes, like in an array
namespace map {

	struct stringmap {
		u32 data_size;
		u32 count;
		array::array keys;
		array::array data;
		u32 next_available;
	};

	//	initialize the map with a defined data size and item count
	stringmap create(u32 data_size, u32 count);
	//	put a piece of data at the next available slot
	void push(stringmap* map, const char* key, void* item);
	//	access an item by its index
	void* at(stringmap* map, u32 index);
	//	access an item by its key
	void* at(stringmap* map, const char* key);
	//	free all memory allocated by the stringmap
	void destroy(stringmap* map);

}
