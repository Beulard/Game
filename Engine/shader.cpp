#include "shader.hpp"
#include "array.hpp"
#include "GL/glew.h"
#include <cstdio>
#include <cstdlib>

bool check_shader_compiled(GLuint id) {
	GLint compiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		printf("Couldn't compile vertex shader. Printing log:\n");
		GLint log_length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = (char*)malloc(log_length);
		glGetShaderInfoLog(id, log_length, NULL, log);
		printf("%s", log);
		free(log);

		return false;
	}
	return true;
}

bool check_program_linked(GLuint id) {
	GLint compiled = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		printf("Couldn't link program. Printing log:\n");
		GLint log_length = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = (char*)malloc(log_length);
		glGetProgramInfoLog(id, log_length, NULL, log);
		printf("%s", log);
		free(log);

		return false;
	}
	return true;
}

shader shader::create(const char* vs, const char* fs) {
	shader s;
	s.id = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//	Compile/link vertex shader
	glShaderSource(vertex_shader, 1, &vs, NULL);
	glCompileShader(vertex_shader);
	if (check_shader_compiled(vertex_shader))
		glAttachShader(s.id, vertex_shader);
	else
		return shader{};

	//	Compile/link fragment shader
	glShaderSource(fragment_shader, 1, &fs, NULL);
	glCompileShader(fragment_shader);
	if (check_shader_compiled(fragment_shader))
		glAttachShader(s.id, fragment_shader);
	else
		return shader{};

	glLinkProgram(s.id);
	if (!check_program_linked(s.id))
		return shader{};

	return s;
}

void shader::destroy() {
	glDeleteShader(id);
}

void shader::use() {
	glUseProgram(id);
}

int shader::get_attrib_location(const char* name) {
	return glGetAttribLocation(id, name);
}

int shader::get_uniform_location(const char* name) {
	return glGetUniformLocation(id, name);
}