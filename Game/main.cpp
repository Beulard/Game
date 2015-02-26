#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"
#include "texture.hpp"
#include "shader.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


GLFWwindow* window;

bool Init(u32 width, u32 height) {
	if (!glfwInit()){
		printf("Couldn't initialize GLFW");
		return false;
	}

	window = glfwCreateWindow(width, height, "Game desu", NULL, NULL);
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
	u32 width = 800, height = 600;

	Init(width, height);

	resource::add_image_png("grass.png");
	resource::add_shader("sprite.vs");
	resource::add_shader("sprite.fs");
	resource::loading_start();

	texture::init(resource::get_image_count());
	shader::init(resource::get_shader_count());


	resource::image_data* grass = resource::get_image("grass.png");
	auto grass_tex = texture::make(grass->bytes, grass->x, grass->y, grass->bpp);


	float projection[] = {
		2/(float)width, 0, 0, -1,
		0, -2/(float)height, 0, 1,
		0, 0, -2, 1,
		0, 0, 0, 1
	};

	resource::shader_data* vertex = resource::get_shader("sprite.vs");
	resource::shader_data* fragment = resource::get_shader("sprite.fs");

	auto shader_sprite = shader::make(vertex->code.c_str(), fragment->code.c_str());

	GLint attrib_coords = shader::get_attrib_location(shader_sprite, "coords");
	GLint attrib_texcoords = shader::get_attrib_location(shader_sprite, "texcoords");

	GLint uniform_color = shader::get_uniform_location(shader_sprite, "color");
	GLint uniform_texture = shader::get_uniform_location(shader_sprite, "texture");
	GLint uniform_projection = shader::get_uniform_location(shader_sprite, "projection");

	glfwSetKeyCallback(window, key_callback);

	//static GLfloat quad[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
	static GLfloat quad[] = { 0, 0, 400, 0, 400, 400, 0, 400 };
	static GLfloat texcoords[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
	static GLfloat color[] = { 1.f, 1.f, 1.f, 0.7f };

	//	upload vertex coordinates
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);

	//	upload texture coordinates
	GLuint texcoords_buffer;
	glGenBuffers(1, &texcoords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);
	
	glActiveTexture(GL_TEXTURE0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(.2, .2, .2, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		//	specify shader program
		shader::use(shader_sprite);

		//	send color
		glUniform4fv(uniform_color, 1, color);

		//	bind and send texture
		texture::bind(grass_tex);
		glUniform1i(uniform_texture, 0);

		//	send projection matrix
		glUniformMatrix4fv(uniform_projection, 1, GL_TRUE, projection);

		//	send vertex attribute
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glEnableVertexAttribArray(attrib_coords);
		glVertexAttribPointer(attrib_coords, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//	send texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_buffer);
		glEnableVertexAttribArray(attrib_texcoords);
		glVertexAttribPointer(attrib_texcoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//	draw
		glDrawArrays(GL_QUADS, 0, 4);
		
		//	disable attribs
		glDisableVertexAttribArray(attrib_texcoords);
		glDisableVertexAttribArray(attrib_coords);

		glfwSwapBuffers(window);
	}
	//	free up gl resources
	//glDeleteProgram(program);
	
	//	clean up
	resource::destroy();
	texture::destroy();
	glfwTerminate();
	return 0;
}