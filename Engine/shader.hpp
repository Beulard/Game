#include "types.hpp"
//#include "GL/glew.h"

namespace shader {

	//	initializes the shader array
	void init(u32 nb_shaders);
	//	creates the shader program with vertex + fragment shaders and returns an id
	int make(const char* vs_code, const char* fs_code);
	//	cleans up all shaders stored by opengl
	void destroy();

	//	tells opengl to use this shader
	void use(u32 id);

	void add_attrib(const char* name);
	//	return the attribute location of an attribute from the shader
	int get_attrib_location(u32 id, const char* name);
	//	same for a uniform variable
	int get_uniform_location(u32 id, const char* name);

}