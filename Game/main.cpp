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

void print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}

int main(int argc, char** argv) {
	u32 width = 800, height = 600;

	Init(width, height);

	resource::add_image_png("grass.png");
	resource::add_shader("vertex.vs");
	resource::add_shader("fragment.fs");
	resource::loading_start();

	float projection[] = {
		2/(float)width, 0, 0, -1,
		0, 2/(float)height, 0, -1,
		0, 0, -2, 1,
		0, 0, 0, 1
	};

	resource::image_data* grass = resource::get_image("grass.png");
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass->x, grass->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass->bytes);

	resource::shader_data* vertex = resource::get_shader("vertex.vs");
	resource::shader_data* fragment = resource::get_shader("fragment.fs");

	const char* vcode = vertex->code.c_str();
	GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_id, 1, &vcode, NULL);
	glCompileShader(v_id);
	print_log(v_id);


	const char* fcode = fragment->code.c_str();
	GLuint f_id;
	f_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_id, 1, &fcode, NULL);
	glCompileShader(f_id);
	print_log(f_id);

	GLuint program = glCreateProgram();
	glAttachShader(program, v_id);
	glAttachShader(program, f_id);
	glLinkProgram(program);
	print_log(program);

	GLint attrib_coords = glGetAttribLocation(program, "coords");
	GLint attrib_color = glGetAttribLocation(program, "color");
	GLint attrib_texcoords = glGetAttribLocation(program, "texcoords");

	GLint uniform_texture = glGetUniformLocation(program, "texture");
	GLint uniform_projection = glGetUniformLocation(program, "projection");

	glfwSetKeyCallback(window, key_callback);

	//static GLfloat quad[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
	static GLfloat quad[] = { 0, 0, 50, 0, 50, 50, 0, 50 };
	static GLfloat color[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	static GLfloat texcoords[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);

	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);

	GLuint texcoords_buffer;
	glGenBuffers(1, &texcoords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);
	
	glActiveTexture(GL_TEXTURE0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		//	specify shader program
		glUseProgram(program);

		//	bind texture
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glUniform1i(uniform_texture, 0);

		glUniformMatrix4fv(uniform_projection, 1, GL_TRUE, projection);

		//	upload vertex attribute
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glEnableVertexAttribArray(attrib_coords);
		glVertexAttribPointer(attrib_coords, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//	upload texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_buffer);
		glEnableVertexAttribArray(attrib_texcoords);
		glVertexAttribPointer(attrib_texcoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//	upload color attribute
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glEnableVertexAttribArray(attrib_color);
		glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//	draw
		glDrawArrays(GL_QUADS, 0, 4);
		
		//	disable attribs
		glDisableVertexAttribArray(attrib_color);
		glDisableVertexAttribArray(attrib_texcoords);
		glDisableVertexAttribArray(attrib_coords);

		glfwSwapBuffers(window);
	}
	//	free up gl resources
	glDeleteProgram(program);
	glDeleteTextures(1, &tex_id);
	
	resource::destroy();
	glfwTerminate();
	return 0;
}