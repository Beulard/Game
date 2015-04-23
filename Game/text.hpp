#pragma once
#include "graphics_types.hpp"
#include "GL/glew.h"
#include "array.hpp"

namespace text {

	//	data used by the shader to render a vertex
	struct vertex_data {
		vertex pos;
		vertex tex;
		color_normalized col;
		color_normalized outline_col;
	};

	//	character data, aka 4 vertices
	struct character {
		vertex_data vertices[4];
	};

	//	a textbatch contains a contiguous array of character data, as well as handles to a texture and a shader
	//	it also holds locations of attributes and uniforms for the shader
	//	it is used for optimized drawing of text by opengl
	struct textbatch {
		u32 texture;
		u32 shader;

		GLuint vbo;
		GLint attrib_coords;
		GLint attrib_texcoords;
		GLint attrib_color;
		GLint attrib_outline_color;
		GLint uniform_texture;
		GLint uniform_projection;

		u32 next_available;
		array characters;
	};

	//	set the projection matrix used by the game. It will be stored and used by the shader
	//	for rendering. The matrix is an array of 16 floats (4x4 matrix)
	void set_projection(float* matrix);

	//	initialize the textbatch array
	void init(u32 count);
	//	create a textbatch with a designated texture and shader program
	int make_batch(u32 count, u32 texture, u32 shader);
	//	clean up every character and batch
	void destroy();

	//	creates a character with provided parameters
	character make(int x = 0, int y = 0, int width = 0, int height = 0, rect subrect = { 0, 0, 0, 0 }, color col = { 0, 0, 0, 0 }, color outlinecol = { 0, 0, 0, 0 });

	//	set a specific character to be drawn in the next call to render_batch
	void draw(character* c, u32 batch);

	//	render the batch
	void render_batch(u32 batch);

}
