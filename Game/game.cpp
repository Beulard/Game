#include "game.hpp"
#include "resource.hpp"

namespace game {

	void init() {
		//resource::image_data* d = resource::get_image("loading_screen.png");
		resource::spritesheet_data* d = resource::get_spritesheet("loading_screen.json");
		resource::shader_data* sd = resource::get_shader("sprite.vs");
	}

	void update() {
	
	}

}