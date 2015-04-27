#pragma once
#include "types.hpp"

class texture {
public:
	//	make a new opengl texture from image data
	static texture create(u8* bytes, int x, int y, int bpp);
	//	free up resources
	void destroy();

	//	bind the texture for opengl usage
	void bind();

	//	return size of texture image
	const vec2i& get_size() const;

private:
	u32 id;
	vec2i size;
};

