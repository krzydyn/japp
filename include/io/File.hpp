#ifndef __IO_FILE_HPP
#define __IO_FILE_HPP

#include <lang/Object.hpp>
#include <iostream>

namespace io {

class File : public Object {
private:
public:
	File(const String& pathname) {
	}
	File(const String& parent, const String& child) {
	}

	static const char separatorChar = '/';
	static const String separator;
};

} //namespace io

#endif
