#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>


void array::set_item_size(u32 size) {
	if (data)
		*(u32*)(((u8*)data + ITEM_SIZE_POS)) = size;
}

void array::set_item_count(u32 count) {
	if(data)
		*(u32*)(((u8*)data + ITEM_COUNT_POS)) = count;
}


array array::create(u32 item_size, u32 item_count) {
	array a;
	a.data = malloc(HEADER_SIZE + item_size * item_count);
	a.set_item_size(item_size);
	a.set_item_count(item_count);
	a.zero_all();
	return a;
}

void array::destroy() {
	free(data);
	data = NULL;
}

void array::resize(u32 count) {
	if (data) {
		data = realloc(data, HEADER_SIZE + count * get_item_size());
		set_item_count(count);
	}
}

void* array::at(u32 index) {
	if(index < get_item_count() && data)
		return ((u8*)data + HEADER_SIZE + index * get_item_size());
	else
		printf("out of bounds !\n");
	return NULL;
}

bool array::is_valid() {
	return !data ? false : true;
}

const u32 array::get_item_size() const {
	if(data)
		return *(u32*)(((u8*)data + ITEM_SIZE_POS));
	return 0;
}

const u32 array::get_item_count() const {
	if(data)
		return *(u32*)(((u8*)data + ITEM_COUNT_POS));
	return 0;
}

void array::zero_all() {
	if(data) 
		memset((u8*)data + HEADER_SIZE, 0, get_item_size() * get_item_count());
}

