#include "sprite.hpp"
#include "GL/glew.h"
#include "shader.hpp"
#include "texture.hpp"
#include <cstring>

namespace sprite {

	static array batches;
	static u32 next_available = 0;

	//	game's projection matrix
	static float projection[16];

	void set_projection(float* matrix) {
		for (u32 i = 0; i < 16; ++i) {
			projection[i] = matrix[i];
		}
	}

	void init(u32 count) {
		//	make the active texture GL_TEXTURE0 if it wasn't already
		glActiveTexture(GL_TEXTURE0);
		//	initialize the batches array
		batches = array::create(sizeof(spritebatch), count);
	}

	int make_batch(u32 count, texture* texture, u32 shader) {
		u32 id = next_available++;
		spritebatch* batch = (spritebatch*)batches[id];
		batch->texture = texture;
		batch->shader = shader;

		glGenBuffers(1, &batch->vbo);
		
		//	assign each attribute and uniform required for rendering
		batch->attrib_coords = shader::get_attrib_location(shader, "coords");
		batch->attrib_texcoords = shader::get_attrib_location(shader, "texcoords");
		batch->attrib_color = shader::get_attrib_location(shader, "color");
		batch->uniform_texture = shader::get_uniform_location(shader, "texture");
		batch->uniform_projection = shader::get_uniform_location(shader, "projection");

		batch->sprites = array::create(sizeof(sprite), count);
		return id;
	}

	void destroy() {
		//	destroy every sprite from every batch
		for (u32 i = 0; i < batches.get_item_count(); ++i) {
			spritebatch* b = (spritebatch*)batches[i];
			glDeleteBuffers(1, &b->vbo);
			b->sprites.destroy();
		}

		//	destroy every batch
		batches.destroy();
	}

	sprite make(int x, int y, int width, int height, rect subrect, color col) {
		#define f(x) (float)x

		return
		//sprite
		{
			//vertex_data
			{ 
				//vertex_data[0]
				{
					//vertex (pos)
					{ f(x), f(y) }, 
					//vertex (tex)
					{ f(subrect.left), f(subrect.top) }, 
					//color_normalized
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f } 
				},
				//vertex_data[1]
				{ 
					{ f(x + width), f(y) }, 
					{ f(subrect.right), f(subrect.top) }, 
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f } 
				},
				//vertex_data[2]
				{ 
					{ f(x + width), f(y + height) }, 
					{ f(subrect.right), f(subrect.bottom) }, 
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f } 
				},
				//vertex_data[3]
				{ 
					{ f(x), f(y + height) }, 
					{ f(subrect.left), f(subrect.bottom) }, 
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f } 
				} 
			} 
		};
		#undef f
	}

	void draw(sprite* s, u32 batch) {
		spritebatch* b = (spritebatch*)batches[batch];
		//	simply copy the data from the sprite to the batch's array
		sprite* dest = (sprite*)b->sprites[b->next_available++];
		memcpy(dest, s, sizeof(sprite));
	}

	void render_batch(u32 batch) {
		spritebatch* b = (spritebatch*)batches[batch];
		//	use opengl to render each sprite from the batch
		shader::use(b->shader);
		b->texture->bind();

		//	normalize texture coordinates
		vec2i tex_size = b->texture->get_size();
		for (u32 i = 0; i < b->sprites.get_item_count(); ++i) {
			sprite* s = (sprite*)b->sprites[i];
			for (u32 j = 0; j < 4; ++j) {
				(float)s->vertices[j].tex.x /= tex_size.x;
				(float)s->vertices[j].tex.y /= tex_size.y;
			}
		}

		//	send texture and projection matrix as uniforms
		glUniform1i(b->uniform_texture, 0);
		glUniformMatrix4fv(b->uniform_projection, 1, GL_FALSE, projection);

		//	upload the vertex coordinates data
		glBindBuffer(GL_ARRAY_BUFFER, b->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sprite) * b->sprites.get_item_count(), b->sprites[0], GL_DYNAMIC_DRAW);

		//	enable attributes
		glEnableVertexAttribArray(b->attrib_coords);
		glEnableVertexAttribArray(b->attrib_texcoords);
		glEnableVertexAttribArray(b->attrib_color);

		//	specify attributes location
		glVertexAttribPointer(b->attrib_coords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);
		glVertexAttribPointer(b->attrib_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)sizeof(vertex));
		glVertexAttribPointer(b->attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)(sizeof(vertex) * 2));
		
		//	draw
		glDrawArrays(GL_QUADS, 0, 4 * b->sprites.get_item_count());

		//	disable attribs
		glDisableVertexAttribArray(b->attrib_color);
		glDisableVertexAttribArray(b->attrib_texcoords);
		glDisableVertexAttribArray(b->attrib_coords);

		//	reset the sprite_batch's sprite array after rendering
		b->sprites.zero_all();
		b->next_available = 0;
	}

}