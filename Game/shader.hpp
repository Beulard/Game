#include "types.hpp"

namespace shader {

	//	Initializes the shader array
	void init(u32 nb_shaders);
	//	Creates the shader program with vertex + fragment shaders and returns an id
	u32 make(const char* vs_code, const char* fs_code);
	//	Binds the shader
	void bind(u32 id);

}