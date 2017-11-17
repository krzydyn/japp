#ifndef __IO_FILEINPUTSTREAM_HPP
#define __IO_FILEINPUTSTREAM_HPP

#include <io/InputStream.hpp>
#include <iostream>
#include <fstream>

namespace io {

class FileInputStream : extends InputStream {
private:
	//use pointer to impl. assign oparation while std::istream assign is protected
	std::istream* in;
	boolean allocated = false;
	boolean closed = true;
	void move(FileInputStream* o) {
		if (o==this) return ;
		in = o->in; o->in = null;
		allocated = o->allocated; o->allocated = false;
	}

public:
	FileInputStream(const FileInputStream& o) = delete;
	FileInputStream& operator=(const FileInputStream& o) = delete;
	FileInputStream(FileInputStream&& o) { move(&o); }
	FileInputStream& operator=(FileInputStream&& o) { move(&o); return *this; }
	~FileInputStream() {
		close();
		if (allocated) delete in;
	}

	FileInputStream(std::istream& s) : in(&s), allocated(false) {}
	FileInputStream(const io::File& f) {
		std::ifstream *fs = new std::ifstream();
		in=fs; allocated=true;
		fs->open(f.getPath().intern(), std::fstream::binary | std::fstream::in);
		if (fs->bad()) throw IOException();
		closed=false;
	}

	using InputStream::read;
	int read() {
		char c;
		in->read(&c,sizeof(char));
		if (in->bad()) throw IOException();
		if (in->gcount()==0) return -1;
		return c;
	}
	int read(void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return 0;
		in->read((char*)b+off,len);
		if (in->bad()) throw IOException();
		return in->gcount();
	}
	void close() {
		if (!closed) {
			((std::ifstream*)in)->close();
			System.out.println("FileInputStream closed");
		}
	}
};

} //namespace io

#endif
