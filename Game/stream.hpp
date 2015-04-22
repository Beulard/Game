#pragma once
#include "array.hpp"

namespace stream {

	enum seek_pos {
		SP_START,
		SP_CURRENT,
		SP_END
	};

	struct stream {
		array::array data;
		unsigned long cursor;
		unsigned long length;
	};

	bool read_from_file(stream* s, const char* filename);
	void read_from_array(stream* s, array::array a);


	//	returns true if the stream's cursor is at end of the array
	bool eos(stream* s);

	//	seek to 'pos' starting from 'from', returns absolute position of cursor
	unsigned long seek(stream* s, long pos, seek_pos from = SP_CURRENT);
	
	//	read a byte, advance cursor
	u8 read_byte(stream* s);
	//	read a char, advance cursor
	char read_char(stream* s);
	//	read a chunk of data of size 'size'
	void* read_chunk(stream* s, unsigned long size);
	//	TODO what is unsigned long ? make custom type


}
