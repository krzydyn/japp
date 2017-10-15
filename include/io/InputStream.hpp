#ifndef __IO_INPUTSTREAM_HPP
#define __IO_INPUTSTREAM_HPP

#include <iostream>

namespace io {

class InputStream {
private:
	std::istream& in;
public:
	InputStream(std::istream& s) : in(s) {}
};

} //namespace io

#endif
