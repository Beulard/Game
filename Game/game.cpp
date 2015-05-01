#include "game.hpp"
#include "resource.hpp"
#include "spritesheet.hpp"

namespace game {

	void init() {
		//resource::image_data* d = resource::get_image("loading_screen.png");
		resource::spritesheet_data* d = resource::get_spritesheet("loading_screen.json");
		resource::shader_data* sd = resource::get_shader("sprite.vs");
		spritesheet s = spritesheet::create(d->description);
		sprite_info* si = s.get_sprite("loading....png");
		printf("%d %d %d %d\n", si->x, si->y, si->w, si->h);
	}

	void update() {

	}

}