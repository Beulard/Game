#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace array {

	array arrays;
	u32 next_id = 0;

	void init(u32 count) {
		arrays = create(sizeof(array), count);
	}

	void destroy() {
		destroy(&arrays);
		next_id = 0;
	}

	void set_item_size(array* a, u32 size) {
		//memset((char*)a->data + ITEM_SIZE_POS, size, sizeof(u32));
		*(u32*)(((u8*)a->data + ITEM_SIZE_POS)) = size;
	}

	void set_item_count(array* a, u32 count) {
		*(u32*)(((u8*)a->data + ITEM_COUNT_POS)) = count;
	}

	int make(u32 item_size, u32 count) {
		u32 id = next_id++;
		array* a = at(&arrays, id);
		*a = create(item_size, count);
		return id;
	}

	array create(u32 item_size, u32 count) {
		array a;
		a.data = malloc(HEADER_SIZE + item_size * count);
		set_item_size(&a, item_size);
		set_item_count(&a, count);
		zero_all(&a);
		return a;
	}

	void destroy(array* a) {
		free(a->data);
	}

	void resize(array* a, u32 count) {
 		a->data = realloc(a->data, HEADER_SIZE + count * get_item_size(a));
		set_item_count(a, count);
	}

	void* at(array* a, u32 index) {
		if(index < get_item_count(a))
			return ((u8*)a->data + HEADER_SIZE + index * get_item_size(a));
		else
			printf("out of bounds !\n");
		return NULL;
	}

	const u32 get_item_size(array* a) {
		return *(u32*)(((u8*)a->data + ITEM_SIZE_POS));
	}

	const u32 get_item_count(array* a) {
		return *(u32*)(((u8*)a->data + ITEM_COUNT_POS));
	}

	void zero_all(array* a) {
		memset((u8*)a->data + HEADER_SIZE, 0, get_item_size(a) * get_item_count(a));
	}

}