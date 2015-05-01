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
		//	8 KB max size
		char code[8192];
	};


	//	fontdesc_data is just an array of bytes that will contain the contents of the binary font description file
	struct fontdesc_data {
		array bytes;
	};

	//	contains the contents of the json description file
	struct spritesheet_data {
		//	16 KB max size
		char description[16384];
	};

	//	The add_* functions add the files to the loading list to get loaded while displaying a loading screen
	void add_image_png(const char* filename);
	//	spritesheets are the output from TexturePacker(C), json files that describe the sprite positions in an image
	//	the name of the image should be the same as the description file
	void add_spritesheet(const char* filename);
	//	
	void add_shader(const char* filename);
	//	
	void add_font(const char* fontdesc, const char* fontimage);
	

	//	returns the total number of items to be loaded
	int get_total_count();


	//	load the resources synchronously
	void load_sync();

	//	start the loading and return, and increments the loading progress variable for every item loaded
	void load_async();
	//	returns only when loading is finished
	void loading_join();
	//	returns how many items have been loaded
	int get_loading_progress();

	//	clears all items that were set to be loaded
	void clear_loading_list();

	//	returns the image loaded and stored at 'name'
	image_data* get_image(const std::string& name);
	//	returns spritesheet
	spritesheet_data* get_spritesheet(const std::string& name);
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