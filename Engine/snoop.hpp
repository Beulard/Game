#pragma once
#define GLEW_STATIC

typedef void(*init_func)();
typedef void(*load_func)();
typedef void(*update_func)();

namespace snoop {

	bool init(const char* window_name, int width, int height);
	void run(init_func i, update_func u);
	void destroy();

}