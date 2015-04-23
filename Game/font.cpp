#include "font.hpp"
#include "stream.hpp"
#include <cstdio>
#include <cstring>

namespace font {

	array fonts;
	u32 next_available = 0;

	void init(u32 count) {
		fonts = array::create(sizeof(font), count);
	}

	void destroy() {
		fonts.destroy();
	}

	int make(array fontdesc, u32 tex) {
		stream::byte_instream font_stream(fontdesc);

		//	check if description has correct format
		char B, M, F, version;
		B = font_stream.read_u8();
		M = font_stream.read_u8();
		F = font_stream.read_u8();
		version = font_stream.read_u8();
		if (B == 'B' && M == 'M' && F == 'F' && version == 3)
			printf("Correct format\n");
		else
			return -1;

		u32 id = next_available++;

		font* f = (font*)fonts[id];
		f->info = { 0 };
		f->common = { 0 };
		u32 nbChars = 0;

		while (1) {
			u8 block_type = font_stream.read_u8();
			u32 block_size = font_stream.read_u32();
			if (block_type > 5) {
				printf("Error reading BMF font description file");
				break;
			}

			switch (block_type) {
				//	info block
			case 1:
				memcpy(&f->info, font_stream.read_chunk(block_size), block_size);
				break;

				//	common block
			case 2:
				memcpy(&f->common, font_stream.read_chunk(block_size), block_size);
				break;

				//	pages block
			case 3:
				//	nothing really interesting here let's just skip it
				font_stream.seek(block_size, SP_CURRENT);
				break;

				//	chars block
			case 4:
				nbChars = block_size / sizeof(font_char);
				f->chars = array::create(sizeof(font_char), nbChars);
				for (u32 i = 0; i < nbChars; ++i) {
					font_char* c = (font_char*)f->chars[i];
					memcpy(c, font_stream.read_chunk(sizeof(font_char)), sizeof(font_char));
				}
				break;

				//	kerning pairs block
			case 5:
				//	meh, skip it
				font_stream.seek(block_size, SP_CURRENT);
				break;
			}
			//	break if we reach end of file
			if (font_stream.eos())
				break;
		}
		return id;
	}

	text::character render_char(u32 id, char c, int x, int y, color col, color outline) {
		font_char* fc = get_char(id, c);
		text::character tc = text::make(x, y, fc->width, fc->height, 
		{ fc->x, fc->y, fc->x + fc->width, fc->y + fc->height }, col, outline);
		return tc;
	}

	font_char* get_char(u32 id, char c) {
		font* f = (font*)fonts[id];
		for (u32 i = 0; i < f->chars.get_item_count(); ++i) {
			font_char* fc = (font_char*)f->chars[i];
			if (c == fc->id) {
				return fc;
			}
		}
		return NULL;
	}

	int get_advance(u32 id, char c) {
		font* f = (font*)fonts[id];
		return get_char(id, c)->xadvance;
	}

}