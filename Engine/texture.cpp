#include "texture.hpp"
#include "GL/glew.h"

texture texture::create(u8* bytes, int x, int y, int bpp) {
	texture tex;
	tex.size = { x, y };
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp > 3 ? GL_RGBA : GL_RGB, x, y, 0, bpp > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, bytes);
	return tex;
}

void texture::destroy() {
	glDeleteTextures(1, &id);
}

void texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

const vec2i& texture::get_size() const {
	return size;
}