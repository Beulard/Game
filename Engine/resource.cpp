#include "resource.hpp"
#include "array.hpp"
#include "file.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <vector>
#include "map.hpp"
#include <thread>

namespace resource {
	
	//	contain the names of resources to be loaded
	std::vector<std::string> image_files;
	std::vector<std::string> shader_files;
	std::vector<std::string> fontdesc_files;

	//	contain the loaded data in string indexed maps
	hashmap image_data_map;
	hashmap shader_data_map;
	hashmap fontdesc_data_map;


	void add_image_png(const char* filename) {
		image_files.push_back(filename);
	}

	void add_shader(const char* filename) {
		shader_files.push_back(filename);
	}

	void add_font(const char* fontdesc, const char* fontimage) {
		image_files.push_back(fontimage);
		fontdesc_files.push_back(fontdesc);
	}

	void load_images(item_callback on_item_loaded, void* param) {
		for (u32 i = 0; i < image_files.size(); ++i){
			//	for each image file added, insert a new entry in the 'image_data_map' and populate the image_data structure
			int x, y, n;
			u8* img_bytes = stbi_load(image_files[i].c_str(), &x, &y, &n, 4);
			image_data data = { img_bytes, x, y, n };
			image_data_map.push(image_files[i].c_str(), &data);
			if (on_item_loaded)
				on_item_loaded(param);
		}
	}

	void load_shaders(item_callback on_item_loaded, void* param) {
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
			shader_data_map.push(shader_files[i].c_str(), &data);
			if (on_item_loaded)
				on_item_loaded(param);
		}
	}

	void load_fontdesc(item_callback on_item_loaded, void* param) {
		for (u32 i = 0; i < fontdesc_files.size(); ++i) {
			fontdesc_data data = { file::read_binary(fontdesc_files[i].c_str()) };
			fontdesc_data_map.push(fontdesc_files[i].c_str(), &data);
			if (on_item_loaded)
				on_item_loaded(param);
		}
	}

	void loading_start(item_callback on_item_loaded, void* item_param, done_callback on_done) {
		//	initialize maps
		image_data_map = hashmap::create(sizeof(image_data), image_files.size());
		shader_data_map = hashmap::create(sizeof(shader_data), shader_files.size());
		fontdesc_data_map = hashmap::create(sizeof(fontdesc_data), fontdesc_files.size());
		//	do the actual loading
		load_images(on_item_loaded, item_param);
		load_shaders(on_item_loaded, item_param);
		load_fontdesc(on_item_loaded, item_param);
		if (on_done)
			on_done();
		clear_loading_list();
	}

	void load_sync() {
		loading_start(NULL, NULL, NULL);
	}

	void images(item_callback on_item_loaded, void* param) {
		for (u32 i = 0; i < 10; ++i) {
			std::chrono::milliseconds sleep(200);
			std::this_thread::sleep_for(sleep);
			if (on_item_loaded)
				on_item_loaded(param);
		}
	}

	void load_async(item_callback on_item_loaded, void* param, done_callback done) {
		std::thread parent(images, on_item_loaded, param);
		parent.join();
	}

	void clear_loading_list() {
		image_files.clear();
		shader_files.clear();
		fontdesc_files.clear();
	}

	image_data* get_image(const std::string& name) {
		return (image_data*)image_data_map[name.c_str()];
	}

	shader_data* get_shader(const std::string& name) {
		return (shader_data*)shader_data_map[name.c_str()];
	}

	fontdesc_data* get_fontdesc(const std::string& name) {
		return (fontdesc_data*)fontdesc_data_map[name.c_str()];
	}

	u32 get_image_count() {
		return image_files.size();
	}

	u32 get_shader_count() {
		return shader_files.size();
	}

	u32 get_font_count() {
		return fontdesc_files.size();
	}

	void destroy() {
		//	free every image loaded by stb
		for (u32 i = 0; i < image_data_map.get_count(); ++i) {
			stbi_image_free(((image_data*)image_data_map[i])->bytes);
		}
		for (u32 i = 0; i < fontdesc_data_map.get_count(); ++i) {
			fontdesc_data* fd = (fontdesc_data*)fontdesc_data_map[i];
			if (fd->bytes.is_valid())
				fd->bytes.destroy();
		}
		image_data_map.destroy();
		shader_data_map.destroy();
		fontdesc_data_map.destroy();
	}

}