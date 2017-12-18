#ifndef __LANG_IOEXCEPTION_HPP
#define __LANG_IOEXCEPTION_HPP

#include <lang/Exception.hpp>

namespace io {

class IOException : extends Exception {
public:
	IOException() : Exception() {}
	IOException(const String& msg) : Exception(msg) {}
	IOException(const String& msg, Throwable *c) : Exception(msg, c) {}
};

class UnsupportedEncodingException : extends IOException {
public:
	UnsupportedEncodingException() : IOException() {}
	UnsupportedEncodingException(const String& msg) : IOException(msg) {}
	UnsupportedEncodingException(const String& msg, Throwable *c) : IOException(msg, c) {}
};

} //namespace io

#endif
