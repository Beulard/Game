#include "map.hpp"
#include <cstring>
#include <cstdio>
#include "hash.hpp"

hashmap hashmap::create(u32 data_size, u32 count) {
	hashmap map;
	map.count = count;
	map.data_size = data_size;
	map.keys = array::create(sizeof(u32), count);
	map.data = array::create(data_size, count);
	map.next_available = 0;
	return map;
}

void hashmap::push(const char* key, void* item) {
	u32 hashed_key = hash::hash(key);
	u32* key_loc = (u32*)keys[next_available];
	void* data_loc = data[next_available++];
	memcpy(key_loc, &hashed_key, sizeof(u32));
	memcpy(data_loc, item, data_size);
}

void* hashmap::at(u32 index) {
	return data[index];
}

void* hashmap::at(const char* key) {
	u32 hashed_key = hash::hash(key);
	for (u32 i = 0; i < count; ++i) {
		if (*(u32*)keys[i] == hashed_key) {
			return data[i];
		}
	}
	printf("Trying to access map '0x%d' at key '%s' : item not found\n", this, key);
	return NULL;
}

void* hashmap::operator[](u32 index) {
	return at(index);
}

void* hashmap::operator[](const char* key) {
	return at(key);
}

u32 hashmap::get_count() const {
	return count;
}

void hashmap::destroy() {
	data.destroy();
	keys.destroy();
}
