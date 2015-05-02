#pragma once

namespace game {

	const struct {
		char* name;
		int width;
		int height;
	} window = { "qwe", 800, 600 };

	void init();
	void update();

}

