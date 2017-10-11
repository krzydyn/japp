#ifndef __EXCEPTIONS_HPP
#define __EXCEPTIONS_HPP

namespace lang {

class UnsupportedOperationException : public std::runtime_error {
public:
	UnsupportedOperationException(std::string msg) : std::runtime_error(msg) {}
};

} //namespace lang

#endif
