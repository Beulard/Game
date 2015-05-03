#include "types.hpp"
//#include "GL/glew.h"

class shader {
public:
	static shader create(const char* vertex_code, const char* fragment_code);
	void destroy();

	//	tells opengl to use this shader
	void use();

	//	return the attribute location of an attribute from the shader
	int get_attrib_location(const char* name);
	//	same for a uniform variable
	int get_uniform_location(const char* name);

private:
	u32 id;
};