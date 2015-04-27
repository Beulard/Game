#include "snoop.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <cstdio>

namespace snoop {

	GLFWwindow* window;
	float projection[16];

	bool init(const char* name, int width, int height) {
		if (!glfwInit()){
			printf("Couldn't initialize GLFW");
			return false;
		}

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window){
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);

		if(glewInit() != GLEW_OK) {
			printf("Couldn't init GLEW");
			glfwTerminate();
			return false;
		}
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		});

		return true;
	}

	void run(init_func i, update_func u) {
		//	run game init function
		i();

		while (!glfwWindowShouldClose(window)) {
			glClearColor(.20f, .47f, .20f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwPollEvents();
			//	run game update function
			u();
			glfwSwapBuffers(window);
		}
	}

	void destroy() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

}