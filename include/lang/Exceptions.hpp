#ifndef __LANG_EXCEPTIONS_HPP
#define __LANG_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include <lang/Types.hpp>

namespace lang {

class Throwable : public std::runtime_error {
public:
	Throwable(std::string msg="") : std::runtime_error(msg) {}
};

class Error : public Throwable {
public:
	Error(std::string msg="") : Throwable(msg) {}
};

class Exception : public Throwable {
public:
	Exception(std::string msg="") : Throwable(msg) {}
};

class OutOfMemoryError : public Error {
public:
	OutOfMemoryError(std::string msg="") : Error(msg) {}
};

class NullPointerException : public Exception {
public:
	NullPointerException(std::string msg="") : Exception(msg) {}
};

class UnsupportedOperationException : public Exception {
public:
	UnsupportedOperationException(std::string msg="") : Exception(msg) {}
};

class IndexOutOfBoundsException : public Exception {
public:
	IndexOutOfBoundsException(std::string msg="") : Exception(msg) {}
	IndexOutOfBoundsException(int i) : Exception("Index "+std::to_string(i)) {}
};

} //namespace lang

#endif
