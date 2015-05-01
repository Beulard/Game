#include "loading.hpp"
#include "sprite.hpp"


loading loading::create(u32 bar_width, u32 max, sprite::sprite* outline, sprite::sprite* bar) {
	loading l;
	l.progress = 0;
	l.max = max;
	l.outline = outline;
	l.bar = bar;
	l.bar_width = bar_width;
	return l;
}

void loading::destroy() {

}

void loading::update(u32 _progress) {
	progress = _progress;
	float p = (float)progress / (float)max;
	
	bar->set_width(p * bar_width);
}

void loading::draw(u32 batch) {
	outline->draw(batch);
	bar->draw(batch);
}