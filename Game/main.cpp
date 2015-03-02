#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "file.hpp"
#include "stringmap.hpp"

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

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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

	//TODO BINARY BITMAP FONT READER
	

	resource::image_data* grass = resource::get_image("grass.png");
	auto grass_tex = texture::make(grass->bytes, grass->x, grass->y, grass->bpp);


	float projection[] = {
		2 / (float)width, 0, 0, 0,
		0, -2 / (float)height, 0, 0,
		0, 0, -2, 0,
		-1, 1, 1, 1
	};

	resource::shader_data* vertex = resource::get_shader("sprite.vs");
	resource::shader_data* fragment = resource::get_shader("sprite.fs");

	auto shader_sprite = shader::make(vertex->code.c_str(), fragment->code.c_str());

	glfwSetKeyCallback(window, key_callback);


	sprite::init(1);
	sprite::set_projection(projection);

	auto batch1 = sprite::make_batch(80 * 60 + 1, grass_tex, shader_sprite);

	auto sprites = array::create(sizeof(sprite::sprite), 80 * 60);
	for (u32 j = 0; j < 60; ++j) {
		for (u32 i = 0; i < 80; ++i) {
			sprite::sprite* s = (sprite::sprite*)array::at(&sprites, j * 80 + i);
			*s = sprite::make(i * 10, j * 10, 10, 10, { 32, 0, 64, 32 }, { 0, 255, 255, 0 });
		}
	}

	auto s1 = sprite::make(200, 200, 200, 200, { 0, 0, 32, 32 }, { 255, 0, 0, 255 });

	while (!glfwWindowShouldClose(window)) {
		glClearColor(.2f, .2f, .2f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		for (u32 i = 0; i < 80 * 60; ++i) {
			sprite::draw((sprite::sprite*)array::at(&sprites, i), batch1);
		}
		sprite::draw(&s1, batch1);
		sprite::render_batch(batch1);

		glfwSwapBuffers(window);
	}	
	//	clean up
	sprite::destroy();
	resource::destroy();
	texture::destroy();
	glfwTerminate();
	return 0;
}