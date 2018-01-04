#ifndef __IO_FILEINPUTSTREAM_HPP
#define __IO_FILEINPUTSTREAM_HPP

#include <io/InputStream.hpp>
#include <iostream>
#include <fstream>

namespace io {

class FileInputStream : extends InputStream {
private:
	//use pointer, assign oparation in std::istream is protected
	std::istream* in;
	String path;
	boolean allocated = false;
	boolean closed = true;
	void move(FileInputStream* o) {
		if (o==this) return ;
		in = o->in; o->in = null;
		path = std::move(o->path);
		allocated = o->allocated; o->allocated = false;
		closed = o->closed; o->closed = false;
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
	FileInputStream(const String& name) : FileInputStream(io::File(name)) {}
	FileInputStream(const io::File& file) {
		if (file.getPath() == null_obj) {
			throw NullPointerException();
		}
		if (file.isInvalid()) {
			throw FileNotFoundException("Invalid file path");
		}
		System.out.println("FileInputStream "+file.getPath());
		std::ifstream *fs = new std::ifstream();
		in=fs; allocated=true;
		fs->open(file.getPath().intern(), std::fstream::binary | std::fstream::in);
		if (in->fail()) throw IOException(file.getPath()+": "+strerror(errno));
		path = file.getPath();
		closed=false;
	}

	using InputStream::read;
	int read() {
		char c;
		in->read(&c,sizeof(char));
		if (in->gcount()==0) return -1;
		if (in->fail()) throw IOException(path+": "+strerror(errno));
		return c;
	}
	int read(void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return 0;
		in->read((char*)b+off,len);
		if (in->bad()) throw IOException(path+": "+strerror(errno));
		return (int)in->gcount();
	}
	long skip(long n) {
		in->ignore(n);
		return (int)in->gcount();
	}
	void close() {
		if (!closed) {
			((std::ifstream*)in)->close();
			System.out.println("FileInputStream closed");
		}
	}
	String toString() const { return InputStream::toString()+":"+path; }
};

} //namespace io

#endif
