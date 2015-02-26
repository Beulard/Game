#include "shader.hpp"
#include "array.hpp"
#include "GL/glew.h"
//	TODO Remove
#include <cstdio>

namespace shader {

	static array::array shader_array;
	static u32 next_available = 0;

	void init(u32 nb_shaders) {
		shader_array = array::create(sizeof(GLuint), nb_shaders);
	}

	bool check_shader_compiled(GLuint id) {
		GLint compiled = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if(compiled != GL_TRUE) {
			printf("Couldn't compile vertex shader");
			//print_shader_log(id);
			return false;
		}
		return true;
	}

	void print_shader_log(GLuint id) {

	}

	u32 make(const char* vs_code, const char* fs_code) {
		u32 id = next_available++;
		printf("remove me : ID = %d\n", id);
		GLuint* glid = (GLuint*)array::at(&shader_array, id);
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

		return id;
	}

}