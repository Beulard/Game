#include "stream.hpp"
#include "file.hpp"

namespace stream {

	byte_instream::byte_instream(array::array a) {
		data = a;
		cursor = 0;
		if (data.data)
			length = array::get_item_count(&a) * array::get_item_size(&a);
	}

	byte_instream::byte_instream(const char* filename) : byte_instream(file::read_binary(filename)) {

	}

	byte_instream::~byte_instream() {
		close();
	}

	bool byte_instream::eos() {
		return cursor >= length;
	}

	void byte_instream::close() {
		array::destroy(&data);
		cursor = 0;
		length = 0;
	}

	void byte_instream::seek(int pos, seek_pos from) {
		switch (from) {
			case SP_START:
				cursor = pos;
			break;
			case SP_CURRENT:
				cursor += pos;
			break;
			case SP_END:
				cursor = length + pos;
			break;
		}
	}

	u32 byte_instream::tell() {
		return cursor;
	}

	void* byte_instream::read_chunk(u32 nb) {
		void* ret = array::at(&data, cursor);
		cursor += nb;
		return ret;
	}

	u8 byte_instream::read_u8() {
		return *(u8*)read_chunk(1);
	}

	char byte_instream::read_char() {
		return *(char*)read_chunk(1);
	}

	u16 byte_instream::read_u16() {
		return *(u16*)read_chunk(2);
	}

	u32 byte_instream::read_u32() {
		return *(u32*)read_chunk(4);
	}

	int byte_instream::read_int() {
		return *(int*)read_chunk(4);

	}

}