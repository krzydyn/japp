#ifndef __IO_FILEOUTPUTSTREAM_HPP
#define __IO_FILEOUTPUTSTREAM_HPP

#include <io/OutputStream.hpp>
#include <iostream>
#include <fstream>

namespace io {

class FileOutputStream : extends OutputStream {
private:
	//use pointer to impl. assign oparation while std::istream assign is protected
	std::ostream* out;
	String fn;
	boolean allocated = false;
	boolean closed = true;

	void move(FileOutputStream* o) {
		if (o==this) return ;
		out = o->out; o->out = null;
		allocated = o->allocated; o->allocated = false;
	}
public:
	FileOutputStream(const FileOutputStream& o) = delete;
	FileOutputStream& operator=(const FileOutputStream& o) = delete;
	FileOutputStream(FileOutputStream&& o) { move(&o); }
	FileOutputStream& operator=(FileOutputStream&& o) { move(&o); return *this; }
	~FileOutputStream() {
		close();
		if (allocated) delete out;
	}

	FileOutputStream(std::ostream& s) : out(&s), allocated(false) {}
	//FileOutputStream(const io::File& f) : FileOutputStream(f,false) {}
	FileOutputStream(const io::File& f, boolean append=false) {
		std::ofstream *fs = new std::ofstream();
		out=fs; allocated=true;
		if (append) fs->open(f.getPath().intern(), std::fstream::binary | std::fstream::out | std::fstream::app);
		else fs->open(f.getPath().intern(), std::fstream::binary | std::fstream::out | std::fstream::trunc);
		if (fs->fail()) throw IOException(f.getPath()+":"+strerror(errno));
		System::out.println("FileOutputStream allocated "+f.getPath());
		fn = f.getPath();
		closed=false;
	}

	using OutputStream::write;
	void write(int b) {
		char c=(char)b;
		out->write(&c,sizeof(char));
		if (out->fail()) throw IOException(fn+": "+strerror(errno));
	}
	void write(void *b, int off, int len) {
		out->write((char*)b+off,len);
		if (out->fail()) throw IOException(fn+": "+strerror(errno));
	}
	void close() {
		if (!closed) {
			((std::ofstream*)out)->close();
			System::out.println("FileOutputStream closed "+fn);
			closed=true;
		}
	}
};

} //namespace io

#endif
