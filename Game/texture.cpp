#include "GL/glew.h"
#include "texture.hpp"
#include "array.hpp"

namespace texture {

	static array textures;
	static u32 next_available = 0;

	void init(u32 count) {
		textures = array::create(sizeof(texture), count);
	}

	int make(u8* bytes, int x, int y, int bpp) {
		u32 id = next_available++;
		texture* tex = (texture*)textures[id];
		tex->size = { x, y };
		glGenTextures(1, &tex->id);
		glBindTexture(GL_TEXTURE_2D, tex->id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, bpp > 3 ? GL_RGBA : GL_RGB, x, y, 0, bpp > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, bytes);
		return id;
	}

	void destroy() {
		glDeleteTextures(textures.get_item_count(), &((texture*)textures[0])->id);
		textures.destroy();
	}

	void bind(u32 id) {
		glBindTexture(GL_TEXTURE_2D, ((texture*)textures[id])->id);
	}

	const vec2i& get_size(u32 id) {
		return ((texture*)textures[id])->size;
	}
}
