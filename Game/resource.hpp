#include "types.hpp"
#include "array.hpp"
#include <string>

namespace resource {

	//	image_data contains pixels extracted from the png file as bytes and image dimensions and bpp value
	struct image_data {
		u8* bytes;
		//	Width, length
		int x, y;
		//	Number of channels, or bytes per pixel
		int bpp;
	};


	enum shader_type {
		//	Vertex shader
		ST_VS,
		//	Fragment shader
		ST_FS
	};

	//	shader_data is a shader type and the GLSL code from the shader source file	
	struct shader_data {
		shader_type type;
		char code[1024];
	};


	//	fontdesc_data is just an array of bytes that will contain the contents of the binary font description file
	struct fontdesc_data {
		array bytes;
	};

	//	The add_* functions add the files to the loading list to get loaded while displaying a loading screen
	void add_image_png(const char* filename);
	//	
	void add_shader(const char* filename);
	//	
	void add_font(const char* fontdesc, const char* fontimage);
	
	//	The load_* functions force the files to get loaded when the call is made
	//void load_shader(const char* filename);

	//	Start/update the loading of files
	void loading_start();

	//	returns the image loaded and stored at 'name'
	image_data* get_image(const std::string& name);
	//	returns the shader loaded and stored at 'name'
	shader_data* get_shader(const std::string& name);
	//	returns the font description with name 'name'
	fontdesc_data* get_fontdesc(const std::string& name);

	//	returns the number of images
	u32 get_image_count();
	//	returns the number of shaders
	u32 get_shader_count();
	//	returns the number of fonts
	u32 get_font_count();
	
	//	clean up all resources
	void destroy();
}