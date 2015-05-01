#pragma once
#include "types.hpp"

namespace sprite {
	class sprite;
}

class loading
{
public:
	static loading create(u32 bar_width, u32 max, sprite::sprite* outline, sprite::sprite* bar);
	void destroy();

	void update(u32 progress);
	void draw(u32 batch);

private:
	//	current progress, maximum progress, and total width of the bar
	u32 progress, max, bar_width;
	sprite::sprite *outline, *bar;

};

