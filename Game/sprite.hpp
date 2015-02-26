#pragma once
#include "types.hpp"
#include "array.hpp"

namespace sprite {

	//	rect represents the five boundaries of a rectangle, and is used as a sprite's sub-rectangle in a texture
	struct rect {
		int left, top, right, bottom;
	};

	struct color {
		u8 r, g, b, a;
	};

	//	sprite data
	struct sprite {
		int x, y;
		rect subrect;
		color col;
	};

	//	a spritebatch contains a contiguous array of sprite data, as well as handles to a texture and a shader
	//	it is used for optimized drawing of sprites by opengl
	struct spritebatch {
		u32 texture;
		u32 shader;
		u32 next_available;
		array::array sprites;
	};

	//	initialize the spritebatch array
	void init(u32 count);
	//	create a sprite batch with a designated texture and shader program
	int make_batch(u32 count, u32 texture, u32 shader);
	//	clean up every sprite and batch
	void destroy();

	//	creates a sprite with provided parameters
	sprite* make(spritebatch* batch, int x = 0, int y = 0, rect subrect = { 0, 0, 0, 0 }, color col = { 0, 0, 0, 0 });

	//	render the batch
	void draw(spritebatch* batch);

}
