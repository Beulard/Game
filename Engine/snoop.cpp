#include "snoop.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"
#include "loading.hpp"
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include <cstdio>

namespace snoop {

	enum {
		GAME_INIT,
		GAME_RUN
	} state;

	GLFWwindow* window;
	float projection[16];
	struct {
		loading screen;
		sprite::sprite outline;
		sprite::sprite bar;
		u32 batch;
	} load;

	//	TODO REMOVE
	u32 gb;

	bool init(const char* name, int width, int height) {
		//	init glfw
		if (!glfwInit()){
			printf("Couldn't initialize GLFW");
			return false;
		}

		//	create window with glfw
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window){
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);

		//	init glew
		if(glewInit() != GLEW_OK) {
			printf("Couldn't init GLEW");
			glfwTerminate();
			return false;
		}
		
		//	enable alpha blending for transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//	initialize projection
		float proj[16] = {
			2 / (float)width, 0, 0, 0,
			0, -2 / (float)height, 0, 0,
			0, 0, -2, 0,
			-1, 1, 1, 1
		};

		//	set key callback
		//	TODO event listener
		glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		});

		//	do some mandatory resource loading
		resource::add_spritesheet("loading_screen.json");
		resource::add_image_png("grass2.png");
		resource::add_shader("sprite.vs");
		resource::add_shader("sprite.fs");
		resource::load_sync();

		spritesheet sheet = spritesheet::create(resource::get_spritesheet("loading_screen.json")->description);

		load.screen = loading::create(width - (int)(width / 5.f) - 10, 29, &load.outline, &load.bar);

		
		sprite_info* out = sheet.get_sprite("loading_out.png");
		sprite_info* bar = sheet.get_sprite("loading_bar.png");
		load.outline = sprite::sprite::create(width / 10.f, height - 70, width - width / 5.f, 60,
		{ out->x, out->y, out->x + out->w, out->y + out->h }, { 0, 0, 0, 0 });
		load.bar = sprite::sprite::create(width / 10.f + 5, height - 65, 0, 50,
		{ bar->x, bar->y, bar->x + bar->w, bar->y + bar->h }, { 0, 0, 0, 0 });

		shader::init(4);
		sprite::init(2);
		sprite::set_projection(proj);
		
		return true;
	}


	void run(init_func i, update_func u) {

		resource::image_data* gi = resource::get_image("grass2.png");
		texture gt = texture::create(gi->bytes, gi->x, gi->y, gi->bpp);

		resource::image_data* d = resource::get_image("loading_screen.png");
		texture tex = texture::create(d->bytes, d->x, d->y, d->bpp);
		u32 sh = shader::make(resource::get_shader("sprite.vs")->code, resource::get_shader("sprite.fs")->code);


		gb = sprite::make_batch(10, &gt, sh);

		load.batch = sprite::make_batch(3, &tex, sh);


		//	call game init function
		i();
		bool loading = false;
		//	if the game wants stuff to be loaded, enter loading mode
		if (resource::get_total_count() > 0) {
			resource::load_async();
			loading = true;
		}

		sprite::sprite as = sprite::sprite::create(0, 0, 500, 400, { 0, 0, 64, 64 }, { 50, 50, 100, 0 });

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwPollEvents();
			
			if (loading){
				load.screen.update(resource::get_loading_progress());
				load.screen.draw(load.batch);
				printf("%d %d\n", resource::get_loading_progress(), resource::get_total_count());

				if (resource::get_loading_progress() >= resource::get_total_count())
					resource::loading_join();

				as.draw(gb);

				sprite::render_batch(load.batch);
				sprite::render_batch(gb);
			}
			else{
				/*	switch (state) {
				case GAME_INIT:
				//	run game init function
				i();
				state = GAME_RUN;
				break;
				case GAME_RUN:
				//	run game update function
				u();
				break;
				}*/

				//	update game
				u();
			}

			//	display
			glfwSwapBuffers(window);
		}
	}

	void destroy() {
		//	free up all loaded resources
		resource::destroy();
		glfwDestroyWindow(window);
		glfwTerminate();
	}

}