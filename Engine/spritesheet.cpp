#include "spritesheet.hpp"
#include "cJSON.h"
#include "sprite.hpp"
#include <cstdio>

spritesheet spritesheet::create(const char* description) {
	spritesheet s;
	cJSON *root = NULL, *frames = NULL;
	root = cJSON_Parse(description);
	if (!root) 
		printf("Invalid json file");

	frames = cJSON_GetObjectItem(root, "frames");
	if (!frames)
		printf("Invalid TexturePacker file");
	
	u32 count = cJSON_GetArraySize(frames);
	s.sprites = hashmap::create(sizeof(sprite_info), count);
	for (u32 i = 0; i < count; ++i) {
		cJSON* item = cJSON_GetArrayItem(frames, i);
		const char* name = cJSON_GetObjectItem(item, "filename")->valuestring;
		cJSON* frame = cJSON_GetObjectItem(item, "frame");
		sprite_info info;
		info.x = cJSON_GetObjectItem(frame, "x")->valueint;
		info.y = cJSON_GetObjectItem(frame, "y")->valueint;
		info.w = cJSON_GetObjectItem(frame, "w")->valueint;
		info.h = cJSON_GetObjectItem(frame, "h")->valueint;

		s.sprites.push(name, &info);
	}

	cJSON_Delete(root);
	return s;
}

void spritesheet::destroy() {
	sprites.destroy();
}

sprite_info* spritesheet::get_sprite(const char* name) {
	return (sprite_info*)sprites[name];
}