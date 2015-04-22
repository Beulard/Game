#pragma once
#include "array.hpp"

enum seek_pos {
	SP_START,
	SP_CURRENT,
	SP_END
};

namespace stream {

	struct stream {
		array::array data;
		u32 cursor;
		u32 length;
		u32 item_size;
	};

	bool read_from_file(stream* s, const char* filename);
	void read_from_array(stream* s, array::array a);


	//	returns true if the stream's cursor is at end of the array
	bool eos(stream* s);

	//	seek to 'pos' starting from 'from', using the size of objects in the array as stride
	void seek(stream* s, int pos, seek_pos from = SP_CURRENT);
	//	returns position of cursor
	u32 tell(stream* s);

	//	read a chunk of data and advances the cursor by 'nb'
	void* read_chunk(stream* s, u32 nb);
	//	read next item from the array and advance cursor by 1
	void* read_next(stream* s);


}
