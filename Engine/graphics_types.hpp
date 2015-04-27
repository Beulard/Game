#pragma once
#include "types.hpp"

struct color {
	u8 r, g, b, a;
};

struct vertex {
	float x, y;
};

//	rect represents the five boundaries of a rectangle, and is used as a sprite's sub-rectangle in a texture
struct rect {
	int left, top, right, bottom;
};

//	RGBA color clamped to the [0.f; 1.f] range
struct color_normalized {
	float r, g, b, a;
};