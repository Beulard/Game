#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "resource.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "file.hpp"
#include "map.hpp"
#include "stream.hpp"

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

	stream::byte_instream font_stream("calibri32.fnt");
	char b, m, f, version;
	b = font_stream.read_u8();
	m = font_stream.read_u8();
	f = font_stream.read_u8();
	version = font_stream.read_u8();
	if (b == 'B' && m == 'M' && f == 'F' && version == 3)
		printf("Correct format\n");

	
	struct font_info {
		u16 fontSize;
		u8 bitField;
		u8 charSet;
		u16 stretchH;
		u8 aa;
		u8 paddingUp;
		u8 paddingRight;
		u8 paddingDown;
		u8 paddingLeft;
		u8 paddingHoriz;
		u8 paddingVert;
		u8 outline;
		char fontName[64];
	};

	struct font_common {
		u16 lineHeight;
		u16 base;
		u16 scaleW;
		u16 scaleH;
		u16 pages;
		u8 bitField;
		u8 alphaChnl;
		u8 redChnl;
		u8 greenChnl;
		u8 blueChnl;
	};

	struct font_char {
		u32 id;
		u16 x;
		u16 y;
		u16 width;
		u16 height;
		u16 xoffset;
		u16 yoffset;
		u16 xadvance;
		u8 page;
		u8 chnl;
	};


	font_info info = { 0 };
	font_common common = { 0 };
	u32 nbChars = 0;
	array chars;
	while (1) {
		u8 block_type = font_stream.read_u8();
		u32 block_size = font_stream.read_u32();
		if (block_type > 5) {
			printf("Error reading BMF font description file");
			break;
		}

		switch (block_type) {
			//	info block
			case 1:
				memcpy(&info, font_stream.read_chunk(block_size), block_size);
			break;
		
			//	common block
			case 2:
				memcpy(&common, font_stream.read_chunk(block_size), block_size);
			break;
		
			//	pages block
			case 3:
				//	nothing really interesting here let's just skip it
				font_stream.seek(block_size, SP_CURRENT);
			break;

			//	chars block
			case 4:
				nbChars = block_size / sizeof(font_char);
				chars = array::create(sizeof(font_char), nbChars);
				for (u32 i = 0; i < nbChars; ++i) {
					font_char* c = (font_char*)chars[i];
					memcpy(c, font_stream.read_chunk(sizeof(font_char)), sizeof(font_char));
				}
			break;
			
			//	kerning pairs block
			case 5:
				//	meh, skip it
				font_stream.seek(block_size, SP_CURRENT);
			break;
		}
		//	break if we reach end of file
		if (font_stream.eos())
			break;
	}


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

	auto shader_sprite = shader::make(vertex->code, fragment->code);

	glfwSetKeyCallback(window, key_callback);


	sprite::init(1);
	sprite::set_projection(projection);

	auto batch1 = sprite::make_batch(80 * 60 + 1, grass_tex, shader_sprite);

	auto sprites = array::create(sizeof(sprite::sprite), 80 * 60);
	for (u32 j = 0; j < 60; ++j) {
		for (u32 i = 0; i < 80; ++i) {
			sprite::sprite* s = (sprite::sprite*)sprites[j * 80 + i];
			*s = sprite::make(i * 10, j * 10, 10, 10, { 32, 0, 64, 32 }, { 0, 255, 255, 0 });
		}
	}

	auto s1 = sprite::make(200, 200, 200, 200, { 0, 0, 32, 32 }, { 255, 0, 0, 255 });

	while (!glfwWindowShouldClose(window)) {
		glClearColor(.2f, .2f, .2f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		for (u32 i = 0; i < 80 * 60; ++i) {
			sprite::draw((sprite::sprite*)sprites[i], batch1);
		}
		sprite::draw(&s1, batch1);
		sprite::render_batch(batch1);

		glfwSwapBuffers(window);
	}	
	//	clean up
	sprites.destroy();

	sprite::destroy();
	resource::destroy();
	texture::destroy();
	glfwTerminate();
	return 0;
}