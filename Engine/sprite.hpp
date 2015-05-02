#pragma once
#include "graphics_types.hpp"
//#include "GL/glew.h"
#include "array.hpp"

class texture;

namespace sprite {


	//	data used by the shader to render a vertex
	struct vertex_data {
		vertex pos;
		vertex tex;
		color_normalized col;
	};

	//	sprite data, aka 4 vertices
	class sprite {
	public :
		vertex_data vertices[4];
	
		//	creates a sprite with provided parameters
		static sprite create(int x = 0, int y = 0, int width = 0, int height = 0, rect subrect = { 0, 0, 0, 0 }, color col = { 0, 0, 0, 0 });
		void move(int x, int y);
		//	set a specific sprite to be drawn in the next call to render_batch
		void draw(u32 batch);
		//void scale(float x, float y);
		void set_width(int width);
		void set_height(int height);
	};

	//	a spritebatch contains a contiguous array of sprite data, as well as handles to a texture and a shader
	//	it also holds locations of attributes and uniforms for the shader
	//	it is used for optimized drawing of sprites by opengl
	struct spritebatch {
		texture* texture;
		u32 shader;

		u32 vbo;
		int attrib_coords;
		int attrib_texcoords;
		int attrib_color;
		int uniform_texture;
		int uniform_projection;

		u32 next_available;
		array sprites;
	};

	//	set the projection matrix used by the game. It will be stored and used by the shader
	//	for rendering. The matrix is an array of 16 floats (4x4 matrix)
	void set_projection(float* matrix);

	//	initialize the spritebatch array
	void init(u32 count);
	//	create a sprite batch with a designated texture and shader program
	int make_batch(u32 count, texture* texture, u32 shader);
	//	clean up every sprite and batch
	void destroy();

	//	render the batch
	void render_batch(u32 batch);

}
