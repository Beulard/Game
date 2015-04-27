#pragma once
#include "array.hpp"
#include "texture.hpp"

//	font uses the BMF tool's binary output to read a set of bitmap characters
//	and be able to render individual characters and their outline
//	
namespace font {

	struct font_info {
		u16 fontSize;
		u8 bitField;
		u8 charSet;
		u16 stretchH;
		u8 aa;
		u8 paddingUp;
		u8 paddingRight;
		u8 paddingDown;
		u8 paddingLeft;
		u8 paddingHoriz;
		u8 paddingVert;
		u8 outline;
		char fontName[64];
	};

	struct font_common {
		u16 lineHeight;
		u16 base;
		u16 scaleW;
		u16 scaleH;
		u16 pages;
		u8 bitField;
		u8 alphaChnl;
		u8 redChnl;
		u8 greenChnl;
		u8 blueChnl;
	};

	struct font_char {
		u32 id;
		u16 x;
		u16 y;
		u16 width;
		u16 height;
		u16 xoffset;
		u16 yoffset;
		u16 xadvance;
		u8 page;
		u8 chnl;
	};

	struct font {
		font_info info;
		font_common common;
		array chars;
		texture* tex;
	};

	//	initialize fonts array
	void init(u32 count);
	//	free all fonts
	void destroy();
	
	//	create a font from its binary description and texture
	int make(array fontdesc, texture* tex);

	//	returns info about the font
	font_info* get_info(u32 id);
	//	returns common stuff about the font
	font_common* get_common(u32 id);
	//	returns font_char data of a character
	font_char* get_char(u32 id, char c);
	//	returns a sprite with the requested character on it
	//text::character render_char(u32 id, char c, int x, int y, float scale, color col, color outline = { 0, 0, 0, 0 });
	//	returns width of the 'space' character
	int get_space_width(u32 font);

}

//	TODO
//class font {};
