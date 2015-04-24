#include "map.hpp"
#include <cstring>
#include <cstdio>

stringmap stringmap::create(u32 data_size, u32 count) {
	stringmap map;
	map.count = count;
	map.data_size = data_size;
	map.keys = array::create(16 * sizeof(char), count);
	map.data = array::create(data_size, count);
	map.next_available = 0;
	return map;
}

void stringmap::push(const char* key, void* item) {
	char* key_loc = (char*)keys[next_available];
	void* data_loc = data[next_available++];
	memcpy(key_loc, key, 16 * sizeof(char));
	memcpy(data_loc, item, data_size);
}

void* stringmap::at(u32 index) {
	return data[index];
}

void* stringmap::at(const char* key) {
	for (u32 i = 0; i < count; ++i) {
		if (!strcmp(key, (const char*)keys[i])) {
			return data[i];
		}
	}
	printf("Trying to access map '%d' at key '%s' : item not found\n", this, key);
	return NULL;
}

void* stringmap::operator[](u32 index) {
	return at(index);
}

void* stringmap::operator[](const char* key) {
	return at(key);
}

u32 stringmap::get_count() const {
	return count;
}

void stringmap::destroy() {
	data.destroy();
	keys.destroy();
}
