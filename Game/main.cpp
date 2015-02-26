#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


GLFWwindow* window;

bool Init() {
	if (!glfwInit()){
		printf("Couldn't initialize GLFW");
		return false;
	}

	window = glfwCreateWindow(800, 600, "Game desu", NULL, NULL);
	if (!window){
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	if(glewInit() == GLEW_OK)
		return true;
	else {
		printf("Couldn't init GLEW");
		glfwTerminate();
		return false;
	}
}

int main(int argc, char** argv) {
	
	Init();

	resource::add_image_png("grass.png");
	resource::add_shader("vertex.vs");
	resource::add_shader("fragment.fs");
	resource::loading_start();


	resource::image_data* grass = resource::get_image("grass.png");
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass->x, grass->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass->bytes);

	resource::shader_data* vertex = resource::get_shader("vertex.vs");
	resource::shader_data* fragment = resource::get_shader("fragment.fs");

	const char* vcode = vertex->code.c_str();
	GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_id, 1, &vcode, NULL);
	glCompileShader(v_id);


	const char* fcode = fragment->code.c_str();
	GLuint f_id;
	f_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_id, 1, &fcode, NULL);
	glCompileShader(f_id);

	GLuint program = glCreateProgram();
	glAttachShader(program, v_id);
	glAttachShader(program, f_id);
	glLinkProgram(program);

	GLint attrib_coords = glGetAttribLocation(program, "coords");


	glfwSetKeyCallback(window, key_callback);

	static GLfloat quad[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25,
								-.75, -.75, -.5, -.75, -.5, -.5, -.75, -.5,
								-1, 1, -.75, 1, -.75, .75, -1, .75 };


	while (!glfwWindowShouldClose(window)) {

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		glUseProgram(program);
		glEnableVertexAttribArray(attrib_coords);

		glVertexAttribPointer(attrib_coords, 2, GL_FLOAT, GL_FALSE, 0, quad);

		glDrawArrays(GL_QUADS, 0, 12);
		glDisableVertexAttribArray(attrib_coords);

		glfwSwapBuffers(window);
	}
	glDeleteProgram(program);
	resource::destroy();
	glfwTerminate();
	return 0;
}