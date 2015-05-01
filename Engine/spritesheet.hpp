#pragma once
#include "array.hpp"
#include "map.hpp"

//	sprite_info contains basic info to create a sprite from a spritesheet
struct sprite_info {
	int x, y, w, h;
};

class spritesheet
{
public:
	static spritesheet create(const char* description);
	void destroy();

	sprite_info* get_sprite(const char* name);

private:
	hashmap sprites;

};

