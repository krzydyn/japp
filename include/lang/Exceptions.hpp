#ifndef __EXCEPTIONS_HPP
#define __EXCEPTIONS_HPP

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

class UnsupportedOperationException : public Exception {
public:
	UnsupportedOperationException(std::string msg="") : Exception(msg) {}
};

class IndexOutOfBoundsException : public Exception {
public:
	IndexOutOfBoundsException(std::string msg="") : Exception(msg) {}
};

} //namespace lang

#endif
