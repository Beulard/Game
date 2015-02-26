#include "types.hpp"
#include <string>

namespace resource {

	struct image_data {
		u8* bytes;
		//	Width, length
		int x, y;
		//	Number of channels
		int n;
	};


	enum shader_type {
		//	Vertex shader
		ST_VS,
		//	Fragment shader
		ST_FS
	};
	struct shader_data {
		shader_type type;
		std::string code;
	};

	//	The add_* functions add the files to the loading list to get loaded while displaying a loading screen
	void add_image_png(const char* filename);
	void add_shader(const char* filename);
	
	//	The load_* functions force the files to get loaded when the call is made
	void load_shader(const char* filename);

	//	Start/update the loading of files
	void loading_start();

	image_data* get_image(const std::string& name);
	shader_data* get_shader(const std::string& name);
	
	void destroy();
}