#ifndef __IO_FILEOUTPUTSTREAM_HPP
#define __IO_FILEOUTPUTSTREAM_HPP

#include <io/OutputStream.hpp>
#include <iostream>

namespace io {

class FileOutputStream : public OutputStream {
private:
	//use pointer to impl. assign oparation while std::istream assign is protected
	std::ostream* out;
public:
	//copy constructor
	FileOutputStream(const FileOutputStream& other) : out(other.out) {}
	//move constructor
	FileOutputStream(FileOutputStream&& other) { out=other.out; other.out=null; }
	//copy assigment
	FileOutputStream& operator=(const FileOutputStream& other) {out=other.out; return *this;}
	//move assigment
	FileOutputStream& operator=(FileOutputStream&& other) {
		if (this != &other) {out=other.out; other.out=null;}
		return *this;
	}

	FileOutputStream(std::ostream& s) : out(&s) {}
	void write(int b) {
		char c=(char)b;
		out->write(&c,sizeof(char));
	}
	void close() {}
};

} //namespace io

#endif
