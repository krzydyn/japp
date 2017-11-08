#ifndef __IO_FILEOUTPUTSTREAM_HPP
#define __IO_FILEOUTPUTSTREAM_HPP

#include <io/OutputStream.hpp>
#include <iostream>
#include <fstream>

namespace io {

class FileOutputStream : public OutputStream {
private:
	//use pointer to impl. assign oparation while std::istream assign is protected
	std::ostream* out;
	boolean allocated = true;
public:
	//FileOutputStream(const FileOutputStream& o) : out(o.out),allocated(false) {}
	FileOutputStream(FileOutputStream&& o) : out(std::move(o.out)),allocated(o.allocated) { o.allocated=false; }
	//FileOutputStream& operator=(const FileOutputStream& other) {out=other.out; return *this;}
	FileOutputStream& operator=(FileOutputStream&& o) {
		if (this != &o) {out=std::move(o.out); allocated=o.allocated; o.allocated=false;}
		return *this;
	}

	FileOutputStream(std::ostream& s) : out(&s) {}
	FileOutputStream(const io::File& f) {}

	using OutputStream::write;
	void write(int b) {
		char c=(char)b;
		out->write(&c,sizeof(char));
	}
	void write(void *b, int off, int len) {
		out->write((char*)b+off,len);
	}
	void close() {
		((std::ofstream*)out)->close();
	}
};

} //namespace io

#endif
