#pragma once
#include "graphics_types.hpp"
//#include "GL/glew.h"
#include "array.hpp"

class texture;

//	text is the equivalent to sprites, but for screen text
//	it allows to draw individual characters to the screen given
//	font information.
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

	//	simple array of characters
	struct string {
		array chars;
	};

	//	a textbatch contains a contiguous array of character data, as well as handles to a texture and a shader
	//	it also holds locations of attributes and uniforms for the shader
	//	it is used for optimized drawing of text by opengl
	struct textbatch {
		texture* texture;
		u32 shader;

		u32 vbo;
		int attrib_coords;
		int attrib_texcoords;
		int attrib_color;
		int attrib_outline_color;
		int uniform_texture;
		int uniform_projection;

		u32 next_available;
		array characters;
	};

	//	set the projection matrix used by the game. It will be stored and used by the shader
	//	for rendering. The matrix is an array of 16 floats (4x4 matrix)
	void set_projection(float* matrix);

	//	initialize the textbatch array
	void init(u32 count);
	//	create a textbatch with a designated texture and shader program
	int make_batch(u32 count, texture* texture, u32 shader);
	//	clean up every character and batch
	void destroy();

	//	creates a character with provided parameters
	//character make(int x = 0, int y = 0, int width = 0, int height = 0, rect subrect = { 0, 0, 0, 0 }, color col = { 0, 0, 0, 0 }, color outlinecol = { 0, 0, 0, 0 });
	character make_char(char c, u32 font, int x, int y, float scale, color col, color outline_col = { 0, 0, 0, 0 });
	string make_string(const char* string, u32 font, int x, int y, float scale, color col, color outline_col = { 0, 0, 0, 0 });

	//	set a specific character to be drawn in the next call to render_batch
	void draw(character* c, u32 batch);
	//	draw a string to the screen on next call to render_batch
	void draw_string(string* s, u32 batch);

	//	render the batch
	void render_batch(u32 batch);

}
