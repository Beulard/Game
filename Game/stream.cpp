#include "stream.hpp"
#include "file.hpp"

namespace stream {

	bool read_from_file(stream* s, const char* filename) {
		read_from_array(file::read_binary(filename));
		if (!s->data.data)
			return false;
		return true;
	}

	void read_from_array(stream* s, array::array a) {
		s->data = a;
		if (s->data.data)
			s->length = array::get_item_count(&a) * array::get_item_size(&a);
	}

	bool eos(stream* s) {
		return s->cursor >= s->length;
	}

	unsigned long seek(stream* s, long pos, seek_pos from) {
		switch (from) {
			case SP_START:
				s->cursor = pos;
			break;
			case SP_CURRENT:
				
		}
	}

	u8 read_byte(stream* s) {
		
		s->cursor += 1;
	}

}