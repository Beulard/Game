#include "shader.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>

namespace shader {

	static array shader_array;
	static u32 next_available = 0;

	void init(u32 nb_shaders) {
		shader_array = array::create(sizeof(GLuint), nb_shaders);
	}

	void destroy() {
		shader_array.destroy();
	}

	bool check_shader_compiled(GLuint id) {
		GLint compiled = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if(compiled != GL_TRUE) {
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

	int make(const char* vs_code, const char* fs_code) {
		u32 id = next_available++;
		GLuint* glid = (GLuint*)shader_array.at(id);
		*glid = glCreateProgram();

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		//	Compile/link vertex shader
		glShaderSource(vertex_shader, 1, &vs_code, NULL);
		glCompileShader(vertex_shader);
		if(check_shader_compiled(vertex_shader))
			glAttachShader(*glid, vertex_shader);
		else
			return -1;

		//	Compile/link fragment shader
		glShaderSource(fragment_shader, 1, &fs_code, NULL);
		glCompileShader(fragment_shader);
		if(check_shader_compiled(fragment_shader))
			glAttachShader(*glid, fragment_shader);
		else
			return -1;

		glLinkProgram(*glid);
		if (!check_program_linked(*glid))
			return -1;

		return id;
	}

	void use(u32 id) {
		glUseProgram(*(GLuint*)shader_array.at(id));
	}

	GLint get_attrib_location(u32 id, const char* name) {
		return glGetAttribLocation(*(GLuint*)shader_array.at(id), name);
	}

	GLint get_uniform_location(u32 id, const char* name) {
		return glGetUniformLocation(*(GLuint*)shader_array.at(id), name);
	}
}