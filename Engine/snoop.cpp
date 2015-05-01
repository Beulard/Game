#include "snoop.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"
#include <cstdio>

namespace snoop {

	enum {
		LOADING,
		GAME_INIT,
		GAME_RUN
	} state;

	GLFWwindow* window;
	float projection[16];

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

		//	set key callback
		//	TODO event listener
		glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		});

		//	do some mandatory resource loading
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::add_image_png("loading_screen.png");
		resource::load_async();
		state = LOADING;

		return true;
	}

	void run(init_func i, update_func u) {

		while (!glfwWindowShouldClose(window)) {
			glClearColor(.20f, .47f, .20f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwPollEvents();

			static int count = resource::get_total_count();
			static int prog = 0;

			switch (state) {
			case LOADING:
				prog = resource::get_loading_progress();
				printf("%d\n", prog);
				//	if loading is over, join the loader thread and go to game init
				if (prog >= count) {
					resource::loading_join();
					state = GAME_INIT;
				}
				break;
			case GAME_INIT:
				//	run game init function
				i();
				state = GAME_RUN;
				break;
			case GAME_RUN:
				//	run game update function
				u();
				break;
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