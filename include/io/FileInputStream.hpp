#ifndef __IO_FILEINPUTSTREAM_HPP
#define __IO_FILEINPUTSTREAM_HPP

#include <io/InputStream.hpp>
#include <iostream>

namespace io {

class FileInputStream : public InputStream {
private:
	//use pointer to impl. assign oparation while std::istream assign is protected
	std::istream* in;
public:
	//copy constructor
	FileInputStream(const FileInputStream& other) : in(other.in) {}
	//move constructor
	FileInputStream(FileInputStream&& other) { in=other.in; other.in=null; }
	//copy assigment
	FileInputStream& operator=(const FileInputStream& other) {in=other.in; return *this;}
	//move assigment
	FileInputStream& operator=(FileInputStream&& other) {
		if (this != &other) {in=other.in; other.in=null;}
		return *this;
	}

	FileInputStream(std::istream& s) : in(&s) {}
	void close() {}
};

} //namespace io

#endif
