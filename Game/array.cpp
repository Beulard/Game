#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>


void array::set_item_size(u32 size) {
	*(u32*)(((u8*)data + ITEM_SIZE_POS)) = size;
}

void array::set_item_count(u32 count) {
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
}

void array::resize(u32 count) {
 	data = realloc(data, HEADER_SIZE + count * get_item_size());
	set_item_count(count);
}

void* array::at(u32 index) {
	if(index < get_item_count())
		return ((u8*)data + HEADER_SIZE + index * get_item_size());
	else
		printf("out of bounds !\n");
	return NULL;
}

const u32 array::get_item_size() const {
	return *(u32*)(((u8*)data + ITEM_SIZE_POS));
}

const u32 array::get_item_count() const {
	return *(u32*)(((u8*)data + ITEM_COUNT_POS));
}

void array::zero_all() {
	memset((u8*)data + HEADER_SIZE, 0, get_item_size() * get_item_count());
}

