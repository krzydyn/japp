#ifndef __NIO_CODERESULT_HPP
#define __NIO_CODERESULT_HPP

#include <nio/Buffer.hpp>

#undef UNDERFLOW
#undef OVERFLOW

namespace nio {
namespace charset {
class CharacterCodingException : extends io::IOException {
	using IOException::IOException;
};

class MalformedInputException : extends CharacterCodingException {
private:
	int inputLength;
public:
	MalformedInputException(int inputLength) : inputLength(inputLength) {}
	int getInputLength() {return inputLength;}
};

class UnmappableCharacterException : extends CharacterCodingException {
private:
	int inputLength;
public:
	UnmappableCharacterException(int inputLength) : inputLength(inputLength) {}
	int getInputLength() {return inputLength;}
};
}

class CoderResult : extends Object {
private:
     static const int CR_UNDERFLOW  = 0;
     static const int CR_OVERFLOW   = 1;
     static const int CR_ERROR_MIN  = 2;
     static const int CR_MALFORMED  = 2;
     static const int CR_UNMAPPABLE = 3;

	int mType;
	int mLength;
	CoderResult(int type, int length) : mType(type), mLength(length) {}

public:
	static CoderResult UNDERFLOW;
	static CoderResult OVERFLOW;
	static CoderResult malformedForLength(int length) {
		return CoderResult(CR_MALFORMED, length);
	}

	String toString() const;
	boolean isUnderflow() const { return mType == CR_UNDERFLOW; }
	boolean isOverflow() const { return mType == CR_OVERFLOW; }
	boolean isError() const { return mType >= CR_ERROR_MIN; }
	boolean isMalformed() const { return mType == CR_MALFORMED; }
	boolean isUnmappable() const { return mType == CR_UNMAPPABLE; }
	/**
	 * @return  The length of the erroneous input, a positive integer
	 */
	int length() const {
		if (!isError()) throw UnsupportedOperationException();
		return mLength;
	}
	void throwException() const {
		switch (mType) {
			case CR_UNDERFLOW:   throw BufferUnderflowException();
			case CR_OVERFLOW:    throw BufferOverflowException();
			case CR_MALFORMED:   throw charset::MalformedInputException(mLength);
			case CR_UNMAPPABLE:  throw charset::UnmappableCharacterException(mLength);
			default: break;
		}
	}
};

}

#endif
