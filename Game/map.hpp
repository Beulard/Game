#pragma once
#include "array.hpp"

//	hashmap describes a string indexed map where the data can be anything
//	and the keys are hashes of the strings the user chooses
//	the data can also be accessed by simple indices, like in an array
class hashmap {

	public:
		//	initialize a map
		static hashmap create(u32 data_size, u32 count);
		//	free memory
		void destroy();
		//	add an item to the map
		void push(const char* key, void* item);

		//	access operators
		void* at(u32 index);
		void* at(const char* key);
		//
		void* operator[](u32 index);
		void* operator[](const char* key);

		//	returns number of items
		u32 get_count() const;

	private:
		u32 data_size;
		u32 count;
		array keys;
		array data;
		u32 next_available;

};
