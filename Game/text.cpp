#include "text.hpp"
#include "GL/glew.h"
#include "shader.hpp"
#include "texture.hpp"
#include "font.hpp"
#include <cstring>
//todo remove
#include <cstdio>

namespace text {

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
		batches = array::create(sizeof(textbatch), count);
	}

	int make_batch(u32 count, u32 texture, u32 shader) {
		u32 id = next_available++;
		textbatch* batch = (textbatch*)batches[id];
		batch->texture = texture;
		batch->shader = shader;

		glGenBuffers(1, &batch->vbo);

		//	assign each attribute and uniform required for rendering
		batch->attrib_coords = shader::get_attrib_location(shader, "coords");
		batch->attrib_texcoords = shader::get_attrib_location(shader, "texcoords");
		batch->attrib_color = shader::get_attrib_location(shader, "color");
		batch->attrib_outline_color = shader::get_attrib_location(shader, "outlinecolor");
		batch->uniform_texture = shader::get_uniform_location(shader, "texture");
		batch->uniform_projection = shader::get_uniform_location(shader, "projection");

		batch->characters = array::create(sizeof(character), count);
		return id;
	}

	void destroy() {
		//	destroy every sprite from every batch
		for (u32 i = 0; i < batches.get_item_count(); ++i) {
			textbatch* b = (textbatch*)batches[i];
			glDeleteBuffers(1, &b->vbo);
			b->characters.destroy();
		}

		//	destroy every batch
		batches.destroy();
	}

	//	helper function to put the right data in the right place when rendering a character from font data
	character make(int x, int y, int width, int height, rect subrect, color col, color outlinecol) {
#define f(x) (float)x

		return
			//character
		{
			//vertex_data
			{
				//vertex_data[0]
				{
					//vertex (pos)
					{ f(x), f(y) },
					//vertex (tex)
					{ f(subrect.left), f(subrect.top) },
					//color_normalized (text color)
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f },
					//color_normalized (outline color)
					{ outlinecol.r / 255.f, outlinecol.g / 255.f, outlinecol.b / 255.f, outlinecol.a / 255.f }
				},
				//vertex_data[1]
				{
					{ f(x + width), f(y) },
					{ f(subrect.right), f(subrect.top) },
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f },
					{ outlinecol.r / 255.f, outlinecol.g / 255.f, outlinecol.b / 255.f, outlinecol.a / 255.f }
				},
				//vertex_data[2]
				{
					{ f(x + width), f(y + height) },
					{ f(subrect.right), f(subrect.bottom) },
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f },
					{ outlinecol.r / 255.f, outlinecol.g / 255.f, outlinecol.b / 255.f, outlinecol.a / 255.f }
				},
				//vertex_data[3]
				{
					{ f(x), f(y + height) },
					{ f(subrect.left), f(subrect.bottom) },
					{ col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f },
					{ outlinecol.r / 255.f, outlinecol.g / 255.f, outlinecol.b / 255.f, outlinecol.a / 255.f }
				}
			}
		};
#undef f
	}

	character make_char(font::font_char* fc, int x, int y, float scale, color col, color outline_col) {
		character tc = make(x, y, (int)(fc->width * scale), (int)(fc->height * scale),
		{ fc->x, fc->y, fc->x + fc->width, fc->y + fc->height }, col, outline_col);
		return tc;
	}

	character make_char(char c, u32 font, int x, int y, float scale, color col, color outline_col) {
		font::font_char* fc = font::get_char(font, c);
		return make_char(fc, x, y, scale, col, outline_col);
	}

	string make_string(const char* strtxt, u32 font, int x, int y, float scale, color col, color outline_col) {
		u32 len = strlen(strtxt);
		string s = { array::create(sizeof(character), len) };
		font::font_common* common = font::get_common(font);
		const int paragraph_x = x;
		int cursor = x;
		for (u32 i = 0; i < len; ++i) {
			if (strtxt[i] == '\n') {
				cursor = paragraph_x;
				y += common->lineHeight;
			} 
			else {
				font::font_char* fc = font::get_char(font, strtxt[i]);
				character* c = (character*)s.chars[i];
				int ypos = y + (int)(scale * (common->base - fc->height));
				*c = make_char(fc, cursor + (__int16)fc->xoffset, y + fc->yoffset * scale - common->base * scale / 3.f, scale, col, outline_col);
				printf("%d\n", (short)fc->yoffset);
				cursor += (int)(scale * fc->xadvance);
			}
		}
		return s;
	}

	void draw(character* c, u32 batch) {
		textbatch* b = (textbatch*)batches[batch];
		//	simply copy the data from the sprite to the batch's array
		character* dest = (character*)b->characters[b->next_available++];
		memcpy(dest, c, sizeof(character));
	}

	void draw_string(string* s, u32 batch) {
		for (u32 i = 0; i < s->chars.get_item_count(); ++i)
			draw((character*)s->chars[i], batch);
	}

	void render_batch(u32 batch) {
		textbatch* b = (textbatch*)batches[batch];
		//	use opengl to render each sprite from the batch
		shader::use(b->shader);
		texture::bind(b->texture);

		//	normalize texture coordinates
		vec2i tex_size = texture::get_size(b->texture);
		for (u32 i = 0; i < b->characters.get_item_count(); ++i) {
			character* c = (character*)b->characters[i];
			for (u32 j = 0; j < 4; ++j) {
				(float)c->vertices[j].tex.x /= tex_size.x;
				(float)c->vertices[j].tex.y /= tex_size.y;
			}
		}

		//	send texture and projection matrix as uniforms
		glUniform1i(b->uniform_texture, 0);
		glUniformMatrix4fv(b->uniform_projection, 1, GL_FALSE, projection);

		//	upload the vertex coordinates data
		glBindBuffer(GL_ARRAY_BUFFER, b->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(character) * b->characters.get_item_count(), b->characters[0], GL_DYNAMIC_DRAW);

		//	enable attributes
		glEnableVertexAttribArray(b->attrib_coords);
		glEnableVertexAttribArray(b->attrib_texcoords);
		glEnableVertexAttribArray(b->attrib_color);
		glEnableVertexAttribArray(b->attrib_outline_color);

		//	specify attributes location
		glVertexAttribPointer(b->attrib_coords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);
		glVertexAttribPointer(b->attrib_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)sizeof(vertex));
		glVertexAttribPointer(b->attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)(sizeof(vertex) * 2));
		glVertexAttribPointer(b->attrib_outline_color, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)(sizeof(vertex) * 2 + sizeof(color_normalized)));

		//	draw
		glDrawArrays(GL_QUADS, 0, 4 * b->characters.get_item_count());

		//	disable attribs
		glDisableVertexAttribArray(b->attrib_outline_color);
		glDisableVertexAttribArray(b->attrib_color);
		glDisableVertexAttribArray(b->attrib_texcoords);
		glDisableVertexAttribArray(b->attrib_coords);

		//	reset the sprite_batch's sprite array after rendering
		b->characters.zero_all();
		b->next_available = 0;
	}

}
