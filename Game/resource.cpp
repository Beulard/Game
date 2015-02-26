#include "resource.hpp"
#include "array.hpp"
#include "file.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <vector>
#include <map>

namespace resource {
	
	//	contain the names of resources to be loaded
	static std::vector<std::string> image_files;
	static std::vector<std::string> shader_files;

	//	contain the loaded data in string indexed maps
	static std::map<std::string, image_data> image_data_map;
	static std::map<std::string, shader_data> shader_data_map;


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
			auto pair = image_data_map.insert(std::make_pair(image_files[i], image_data{ img_bytes, x, y, n }));
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
			auto pair = shader_data_map.insert(std::make_pair(shader_files[i], shader_data{ st, file::read_all(shader_files[i].c_str()) }));
		}
	}

	void loading_start() {
		//	do the actual loading
		load_images();
		load_shaders();
	}

	image_data* get_image(const std::string& name) {
		return &image_data_map[name];
	}

	shader_data* get_shader(const std::string& name) {
		return &shader_data_map[name];
	}

	void destroy() {
		//	free every image loaded by stb
		for (auto it = image_data_map.begin(); it != image_data_map.end(); ++it) {
			stbi_image_free(it->second.bytes);
		}
	}

}