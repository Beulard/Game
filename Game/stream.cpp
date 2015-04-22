#include "stream.hpp"
#include "file.hpp"

namespace stream {

	bool read_from_file(stream* s, const char* filename) {
		read_from_array(s, file::read_binary(filename));
		if (!s->data.data)
			return false;
		return true;
	}

	void read_from_array(stream* s, array::array a) {
		s->data = a;
		s->cursor = 0;
		if (s->data.data)
			s->length = array::get_item_count(&a);
	}

	bool eos(stream* s) {
		return s->cursor >= s->length;
	}

	void seek(stream* s, int pos, seek_pos from) {
		switch (from) {
			case SP_START:
				s->cursor = pos;
			break;
			case SP_CURRENT:
				s->cursor += pos;
			break;
			case SP_END:
				s->cursor = s->length + pos;
			break;
		}
	}

	u32 tell(stream* s) {
		return s->cursor;
	}

	void* read_chunk(stream* s, u32 nb) {
		void* ret = array::at(&s->data, s->cursor);
		s->cursor += nb;
		return ret;
	}

	void* read_next(stream* s) {
		return read_chunk(s, 1);
	}

}