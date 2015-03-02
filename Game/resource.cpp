#include "resource.hpp"
#include "array.hpp"
#include "file.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <vector>
#include "stringmap.hpp"

namespace resource {
	
	//	contain the names of resources to be loaded
	static std::vector<std::string> image_files;
	static std::vector<std::string> shader_files;

	//	contain the loaded data in string indexed maps
	static stringmap::stringmap image_data_map;
	static stringmap::stringmap shader_data_map;


	void add_image_png(const char* filename) {
		image_files.push_back(filename);
	}

	void add_shader(const char* filename) {
		shader_files.push_back(filename);
	}

	void load_images() {

		for (u32 i = 0; i < image_files.size(); ++i){
			//	for each image file added, insert a new entry in the 'image_data_map' and populate the image_data structure
			int x, y, n;
			u8* img_bytes = stbi_load(image_files[i].c_str(), &x, &y, &n, 4);
			image_data data = { img_bytes, x, y, n };
			stringmap::push(&image_data_map, image_files[i].c_str(), &data);
		}
	}

	void load_shaders() {
		for (u32 i = 0; i < shader_files.size(); ++i) {
			shader_type st = ST_VS;
			if (shader_files[i][shader_files[i].size() - 2] == 'v')
				st = ST_VS;
			else if (shader_files[i][shader_files[i].size() - 2] == 'f')
				st = ST_FS;
			else
				printf("%s : not a valid shader type", shader_files[i].c_str());
			std::string code = file::read_all(shader_files[i].c_str());
			shader_data data;
			data.type = st;
			strcpy_s(data.code, 1024, code.c_str());
			stringmap::push(&shader_data_map, shader_files[i].c_str(), &data);
		}
	}

	void loading_start() {
		//	initialize maps
		image_data_map = stringmap::create(sizeof(image_data), image_files.size());
		shader_data_map = stringmap::create(sizeof(shader_data), shader_files.size());
		//	do the actual loading
		load_images();
		load_shaders();
	}

	image_data* get_image(const std::string& name) {
		return (image_data*)stringmap::at(&image_data_map, name.c_str());
	}

	shader_data* get_shader(const std::string& name) {
		return (shader_data*)stringmap::at(&shader_data_map, name.c_str());
	}

	u32 get_image_count() {
		return image_files.size();
	}

	u32 get_shader_count() {
		return shader_files.size();
	}

	void destroy() {
		//	free every image loaded by stb
		for (u32 i = 0; i < image_data_map.count; ++i) {
			stbi_image_free(((image_data*)stringmap::at(&image_data_map, i))->bytes);
		}
	}

}