#pragma once
#include "types.hpp"
#include "GL/glew.h"
#include "array.hpp"

namespace sprite {


	struct vertex {
		float x, y;
	};

	//	rect represents the five boundaries of a rectangle, and is used as a sprite's sub-rectangle in a texture
	struct rect {
		int left, top, right, bottom;
	};

	struct color {
		u8 r, g, b, a;
	};

	//	RGBA color clamped to the [0.f; 1.f] range
	struct color_normalized {
		float r, g, b, a;
	};

	//	data used by the shader to render a vertex
	struct vertex_data {
		vertex pos;
		vertex tex;
		color_normalized col;
	};

	//	sprite data, aka 4 vertices worth of data
	struct sprite {
		vertex_data vertices[4];
	};

	//	a spritebatch contains a contiguous array of sprite data, as well as handles to a texture and a shader
	//	it also holds locations of attributes and uniforms for the shader
	//	it is used for optimized drawing of sprites by opengl
	struct spritebatch {
		u32 texture;
		u32 shader;

		GLuint vbo;
		GLint attrib_coords;
		GLint attrib_texcoords;
		GLint attrib_color;
		GLint uniform_texture;
		GLint uniform_projection;

		u32 next_available;
		array::array sprites;
	};

	//	set the projection matrix used by the game. It will be stored and used by the shader
	//	for rendering. The matrix is an array of 16 floats (4x4 matrix)
	void set_projection(float* matrix);

	//	initialize the spritebatch array
	void init(u32 count);
	//	create a sprite batch with a designated texture and shader program
	int make_batch(u32 count, u32 texture, u32 shader);
	//	clean up every sprite and batch
	void destroy();

	//	creates a sprite with provided parameters
	sprite make(int x = 0, int y = 0, int width = 0, int height = 0, rect subrect = { 0, 0, 0, 0 }, color col = { 0, 0, 0, 0 });

	//	set a specific sprite to be drawn in the next call to render_batch
	void draw(sprite* s, u32 batch);

	//	render the batch
	void render_batch(u32 batch);

}
