#pragma once
#include "types.hpp"

namespace texture {

	//	initialize the texture array
	void init(u32 count);
	//	create a texture from image data
	int make(u8* bytes, int x, int y, int bpp);
	//	clean up all textures stored by opengl
	void destroy();

	//	bind the texture for opengl usage
	void bind(u32 id);

}

