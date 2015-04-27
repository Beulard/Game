#include <iostream>
//#include "GL/glew.h"
//#include "GLFW/glfw3.h"
#include "resource.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "file.hpp"
#include "map.hpp"
#include "stream.hpp"
#include "font.hpp"
#include "hash.hpp"
#include "text.hpp"
#include "snoop.hpp"
#include "game.hpp"

/*void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}*/


/*GLFWwindow* window;

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

void on_item_loaded(void* progress) {
	int* p = (int*)progress;
	printf("Loading progress: %d\n", ++(*p));
}

void on_done() {
	printf("Done!\n");
}

void item(void* var) {
	resource::mutexed_var<int>* p = (resource::mutexed_var<int>*)var;
	p->m.lock();
	p->var++;
	printf("%d\n", p->var);
	p->m.unlock();
}*/

int main(int argc, char** argv) {
	u32 width = 800, height = 600;
	
	snoop::init("snoop game", width, height);
	snoop::run(&game::init, &game::update);
	snoop::destroy();

	//	TODO wtf is this
	//Init(width, height);

	//	TODO MAKE IT OOP, WITHOUT CONSTRUCTORS
	//	TODO change create, make, etc. to something consistent like new
	//	TODO freetype
	//	TODO freetype outline
	//	TODO organize
	//	TODO better rendering process somehow
	
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resource::add_image_png("grass2.png");
	resource::add_image_png("tex.png");
	resource::add_shader("sprite.vs");
	resource::add_shader("sprite.fs");
	resource::add_shader("text.vs");
	resource::add_shader("text.fs");
	resource::add_font("calibri92.fnt", "calibri92_0.png");

	int progress = 0;

	resource::mutexed_var<int> prog;
	prog.var = 0;
	
	resource::loading_start(&on_item_loaded, &progress, &on_done);
	resource::load_async(item, &prog, NULL);

	shader::init(resource::get_shader_count());
	font::init(resource::get_font_count());

	resource::shader_data* sprite_vertex = resource::get_shader("sprite.vs");
	resource::shader_data* sprite_fragment = resource::get_shader("sprite.fs");

	auto shader_sprite = shader::make(sprite_vertex->code, sprite_fragment->code);

	resource::image_data* font_calibri_img = resource::get_image("calibri92_0.png");
	texture font_calibri_tex = texture::create(font_calibri_img->bytes, font_calibri_img->x, font_calibri_img->y, font_calibri_img->bpp);
	resource::fontdesc_data* font_calibri_desc = resource::get_fontdesc("calibri92.fnt");

	resource::shader_data* text_vertex = resource::get_shader("text.vs");
	resource::shader_data* text_fragment = resource::get_shader("text.fs");
	auto shader_text = shader::make(text_vertex->code, text_fragment->code);

	u32 font_calibri = font::make(font_calibri_desc->bytes, &font_calibri_tex);

	float projection[] = {
		2 / (float)width, 0, 0, 0,
		0, -2 / (float)height, 0, 0,
		0, 0, -2, 0,
		-1, 1, 1, 1
	};

	const color black = { 0, 0, 0, 255 };
	const color white = { 255, 255, 255, 255 };

	text::init(1);
	text::set_projection(projection);
	text::character calA = text::make_char(' ', font_calibri, 100, 100, 1.f, { 255, 255, 255, 255 }, { 0, 0, 0, 255 });
	int text_batch = text::make_batch(128, &font_calibri_tex, shader_text);
	
	text::string hello_world = text::make_string("And so it is that the knight\nwas defeated by the\nforces of evil...", font_calibri, 10, 1, 1.f, white, { 255, 255, 255, 0 });


	resource::image_data* grass = resource::get_image("grass2.png");
	auto grass_tex = texture::create(grass->bytes, grass->x, grass->y, grass->bpp);


	glfwSetKeyCallback(window, key_callback);


	sprite::init(1);
	sprite::set_projection(projection);

	auto batch1 = sprite::make_batch(80 * 60 + 1, &grass_tex, shader_sprite);

	auto sprites = array::create(sizeof(sprite::sprite), 80 * 60);
	for (u32 j = 0; j < 60; ++j) {
		for (u32 i = 0; i < 80; ++i) {
			sprite::sprite* s = (sprite::sprite*)sprites[j * 80 + i];
			*s = sprite::make(i * 10, j * 10, 10, 10, { 32, 0, 64, 32 }, { 0, 0, 255, 0 });
		}
	}
	auto s1 = sprite::make(200, 200, 200, 200, { 0, 0, 32, 32 }, { 100, 100, 50, 150 });

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		for (u32 i = 0; i < 80 * 60; ++i) {
			sprite::draw((sprite::sprite*)sprites[i], batch1);
		}
		sprite::draw(&s1, batch1);
		sprite::render_batch(batch1);

		text::draw(&calA, text_batch);
		text::draw_string(&hello_world, text_batch);
		text::render_batch(text_batch);

		glfwSwapBuffers(window);
	}	
	//	clean up
	sprites.destroy();

	text::destroy();
	font::destroy(); 
	sprite::destroy();
	resource::destroy();
	glfwTerminate();*/
	return 0;
}