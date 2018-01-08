#ifndef __NIO_CODERESULT_HPP
#define __NIO_CODERESULT_HPP

#include <lang/Object.hpp>

#undef UNDERFLOW
#undef OVERFLOW

namespace nio {

class CoderResult : extends Object {
private:
     static const int CR_UNDERFLOW  = 0;
     static const int CR_OVERFLOW   = 1;
     static const int CR_ERROR_MIN  = 2;
     static const int CR_MALFORMED  = 2;
     static const int CR_UNMAPPABLE = 3;

	int type;
	int length;
	CoderResult(int type, int length) : type(type), length(length) {}

public:
	static CoderResult UNDERFLOW;
	static CoderResult OVERFLOW;

	String toString();
	boolean isUnderflow() { return type == CR_UNDERFLOW; }
	boolean isOverflow() { return type == CR_OVERFLOW; }
	boolean isError() { return type >= CR_ERROR_MIN; }
	boolean isMalformed() { return type == CR_MALFORMED; }
	boolean isUnmappable() { return type == CR_UNMAPPABLE; }
	void throwException() {
	}
};

}


#endif
