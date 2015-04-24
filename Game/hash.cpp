#include "hash.hpp"
//	TODO REMOVE
#include <cstdio>

namespace hash {

	u32 m = 15485863;

	u32 hash(const char* string) {
		u32 h = 0;
		const u8* c = (const u8*)string;
		while (*c != '\0') {
			u8 highorder = h & 0xf8000000;
			h = h << 5;
			h = h ^ (highorder >> 27);
			h = h ^ *c;
			c++;
		}
		return h % m;
	}

}