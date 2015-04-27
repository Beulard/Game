#pragma once
#include "array.hpp"

enum seek_pos {
	SP_START,
	SP_CURRENT,
	SP_END
};

namespace stream {

	class stream {
		public:
			virtual void seek(int pos, seek_pos from) = 0;
			virtual u32 tell() = 0;
	};

	//	typical input stream, can read anything from anywhere
	class instream : public stream {
		virtual void* read_chunk(u32 size) = 0;
	};

	//	typical output stream, can write anything to anywhere
	class outstream : public stream {
		virtual void write_chunk(void* data, u32 size) = 0;
	};

	//	byte_instream reads an array of binary data and has built in functions to read
	//	some of the basic types. it can either read directly from an array or from a file
	//	it can also be used to read more types but an explicit conversion is needed each time (see read_chunk)
	class byte_instream : public stream {
		public:
			//	set the array to be read from by the stream
			byte_instream(array a);
			//	read data from a file
			//	the stream should be closed after being used
			//	a stream that reads from a file always has an item size of 1
			byte_instream(const char* filename);

			//	/!\ the destructor WILL free memory from the array it read from
			~byte_instream();

			//	returns true if cursor is at end of source
			bool eos();

			//	seek to 'pos' starting from 'from', using the size of objects in the array as stride
			void seek(int pos, seek_pos from = SP_CURRENT);
			//	returns position of cursor
			u32 tell();

			//	read a chunk of data and advances the cursor by 'size'
			void* read_chunk(u32 size);
			//	read next item from the array and advance cursor by 1
			u8 read_u8();
			char read_char();
			u16 read_u16();
			u32 read_u32();
			int read_int();


		private:
			//	free memory used by the array
			void close();
			
			//	member variables
			//	the array of binary data
			array data;
			//	position in the source
			u32 cursor;
			//	length of the source
			u32 length;
	};

}
