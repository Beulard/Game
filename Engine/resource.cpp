#include "resource.hpp"
#include "array.hpp"
#include "file.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <vector>
#include "map.hpp"
#include <thread>
#include <atomic>

namespace resource {
	
	//	contain the names of resources to be loaded
	std::vector<std::string> image_files;
	std::vector<std::string> shader_files;
	std::vector<std::string> fontdesc_files;
	std::vector<std::string> spritesheet_files;

	//	contain the loaded data in string indexed maps
	hashmap image_data_map;
	hashmap shader_data_map;
	hashmap fontdesc_data_map;
	hashmap spritesheet_data_map;

	const int threads_count = 4;
	std::thread loader_threads[threads_count];
	std::atomic_int loading_progress;

	const std::string data_folder = "data/";


	void add_image_png(const char* filename) {
		image_files.push_back(data_folder + filename);
	}

	void add_spritesheet(const char* filename) {
		std::string image_name = filename;
		//	strip the extension of the .json file
		image_name.resize(image_name.size() - 5);
		//	add the .png extension
		image_name.append(".png");
		image_files.push_back(data_folder + image_name);
		spritesheet_files.push_back(data_folder + filename);
	}

	void add_shader(const char* filename) {
		shader_files.push_back(data_folder + filename);
	}

	void add_font(const char* fontdesc, const char* fontimage) {
		image_files.push_back(data_folder + fontimage);
		fontdesc_files.push_back(data_folder + fontdesc);
	}

	void load_images() {
		for (u32 i = 0; i < image_files.size(); ++i){
			//	for each image file added, insert a new entry in the 'image_data_map' and populate the image_data structure
			int x, y, n;
			u8* img_bytes = stbi_load(image_files[i].c_str(), &x, &y, &n, 4);
			image_data data = { img_bytes, x, y, n };
			image_data_map.push(image_files[i].c_str(), &data);
			loading_progress++;
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
			shader_data_map.push(shader_files[i].c_str(), &data);
			loading_progress++;
		}
	}

	void load_fontdescs() {
		for (u32 i = 0; i < fontdesc_files.size(); ++i) {
			fontdesc_data data = { file::read_binary(fontdesc_files[i].c_str()) };
			fontdesc_data_map.push(fontdesc_files[i].c_str(), &data);
			loading_progress++;
		}
	}

	void load_spritesheets() {
		for (u32 i = 0; i < spritesheet_files.size(); ++i) {
			std::string desc = file::read_all(spritesheet_files[i].c_str());
			spritesheet_data data;
			strcpy_s(data.description, desc.size()+1, desc.c_str());
			spritesheet_data_map.push(spritesheet_files[i].c_str(), &data);
			loading_progress++;
		}
	}

	void loading_start() {
		//	initialize maps
		image_data_map = hashmap::create(sizeof(image_data), image_files.size());
		shader_data_map = hashmap::create(sizeof(shader_data), shader_files.size());
		fontdesc_data_map = hashmap::create(sizeof(fontdesc_data), fontdesc_files.size());
		spritesheet_data_map = hashmap::create(sizeof(spritesheet_data), spritesheet_files.size());
		//	do the actual loading
		load_images();
		load_shaders();
		load_fontdescs();
		load_spritesheets();

		clear_loading_list();
	}

	void load_sync() {
		loading_start();
	}

	void images() {
		for (u32 i = 0; i < image_files.size(); ++i) {
			std::chrono::milliseconds sleep(200);
			std::this_thread::sleep_for(sleep);
			loading_progress++;
		}
	}

	int get_total_count() {
		return image_files.size() + shader_files.size() + fontdesc_files.size();
	}

	void load_async() {
		//	initialize maps
		image_data_map = hashmap::create(sizeof(image_data), image_files.size());
		shader_data_map = hashmap::create(sizeof(shader_data), shader_files.size());
		fontdesc_data_map = hashmap::create(sizeof(fontdesc_data), fontdesc_files.size());
		spritesheet_data_map = hashmap::create(sizeof(spritesheet_data), spritesheet_files.size());

		loading_progress = 0;
		//	start the loader threads
		loader_threads[0] = std::thread(load_images);
		loader_threads[1] = std::thread(load_shaders);
		loader_threads[2] = std::thread(load_fontdescs);
		loader_threads[2] = std::thread(load_spritesheets);
	}

	void loading_join() {
		for (u32 i = 0; i < threads_count; ++i)
			if (loader_threads[i].joinable())
				loader_threads[i].join();
	}

	int get_loading_progress() {
		return loading_progress.load();
	}

	void clear_loading_list() {
		image_files.clear();
		shader_files.clear();
		fontdesc_files.clear();
		spritesheet_files.clear();
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

	spritesheet_data* get_spritesheet(const std::string& name) {
		return (spritesheet_data*)spritesheet_data_map[name.c_str()];
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

	u32 get_spritesheet_count() {
		return spritesheet_files.size();
	}

	void destroy() {
		//	make sure the loader threads have been joined (prevents crashes)
		loading_join();
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
		spritesheet_data_map.destroy();
	}

}