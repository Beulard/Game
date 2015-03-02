#include "stringmap.hpp"
#include <cstring>

namespace stringmap {
	
	stringmap create(u32 data_size, u32 count) {
		stringmap map;
		map.count = count;
		map.data_size = data_size;
		map.keys = array::create(16 * sizeof(char), count);
		map.data = array::create(data_size, count);
		map.next_available = 0;
		return map;
	}

	void push(stringmap* map, const char* key, void* item) {
		char* key_loc = (char*)array::at(&map->keys, map->next_available);
		void* data_loc = array::at(&map->data, map->next_available++);
		memcpy(key_loc, key, 16 * sizeof(char));
		memcpy(data_loc, item, map->data_size);
	}

	void* at(stringmap* map, u32 index) {
		return array::at(&map->data, index);
	}

	void* at(stringmap* map, const char* key) {
		for (u32 i = 0; i < map->count; ++i) {
			if (!strcmp(key, (const char*)array::at(&map->keys, i))) {
				return array::at(&map->data, i);
			}
		}
		return NULL;
	}

	void destroy(stringmap* map) {
		array::destroy(&map->data);
		array::destroy(&map->keys);
	}
}