#include "sprite.hpp"
#include "GL/glew.h"

namespace sprite {

	static array::array batches;
	static u32 next_available = 0;

	void init(u32 count) {
		glActiveTexture(GL_TEXTURE0);
		batches = array::create(sizeof(spritebatch), count);
	}

	int make_batch(u32 count, u32 texture, u32 shader) {
		u32 id = next_available++;
		spritebatch* batch = (spritebatch*)array::at(&batches, id);
		batch->texture = texture;
		batch->shader = shader;
		batch->sprites = array::create(sizeof(sprite), count);
		return id;
	}

	void destroy() {
		//	destroy every sprite from every batch
		for (u32 i = 0; i < array::get_item_count(&batches); ++i) 
			array::destroy(&((spritebatch*)array::at(&batches, i))->sprites);
		
		//	destroy every batch
		array::destroy(&batches);
	}

	sprite* make_sprite(spritebatch* batch, int x, int y, rect subrect, color col) {
		u32 id = batch->next_available++;
		sprite* s = (sprite*)array::at(&batch->sprites, id);
		s->x = x;
		s->y = y;
		s->subrect = subrect;
		s->col = col;
		return s;
	}

	void draw(spritebatch* batch) {
		//	TODO
	}

}