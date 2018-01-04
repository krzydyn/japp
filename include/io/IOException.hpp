#ifndef __LANG_IOEXCEPTION_HPP
#define __LANG_IOEXCEPTION_HPP

#include <lang/Exception.hpp>

namespace io {

class IOException : extends Exception {
public:
	using Exception::Exception;
};

class UnsupportedEncodingException : extends IOException {
public:
	using IOException::IOException;
};

class FileNotFoundException : extends IOException {
public:
	using IOException::IOException;
};

} //namespace io

#endif
