#pragma once
#include "types.hpp"


//	The array struct contains a continuous memory chunk in which lie
//	the size of items, the item count and the actual data
//	The structure of the memory chunk goes like this :
//	Bytes 0-3 : item size (unsigned int)
//	Bytes 4-7 : item count (unsigned int)
//	Bytes after 8 : data

enum {
	ITEM_SIZE_POS = 0,
	ITEM_COUNT_POS = 4,
	HEADER_SIZE = 8,
	DATA_POS = 8
};


class array {
	public:
		//	allocate an array with specified size and count
		static array create(u32 item_size, u32 item_count);
		//	free memory allocated for this array
		void destroy();
		//	resize the array
		void resize(u32 count);
		//	returns the item at 'index' as void*
		void* at(u32 index);

		//	returns true if array was allocated
		bool is_valid();

		//	returns number of items
		const u32 get_item_count() const;
		//	returns item sizes
		const u32 get_item_size() const;
		//	zero every item in the array
		void zero_all();

	private:
		void* data;

		void set_item_size(u32 size);
		void set_item_count(u32 count);
};
