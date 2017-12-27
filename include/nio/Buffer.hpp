#ifndef __NIO_BUFFER_HPP
#define __NIO_BUFFER_HPP

#include <lang/Comparable.hpp>
#include <io/Appendable.hpp>
#include <io/Readable.hpp>

namespace nio {

class InvalidMarkException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class Buffer : extends Object {
private:
	// mXxxxx = member Xxxxx
	int mMark = -1;
	int mPosition = 0;
	int mLimit;
	int mCapacity;

	Buffer(int mark, int pos, int lim, int cap) : mCapacity(cap) {
		if (cap < 0) throw IllegalArgumentException("Negative capacity: " + String::valueOf(cap));
		limit(lim);
		position(pos);
		if (mark >= 0) {
			if (mark > pos) throw IllegalArgumentException("mark > position");
			this->mMark = mark;
		}
	}

public:
	int capacity() const { return mCapacity; }
	int position() const { return mPosition; }
	Buffer& position(int newPosition) {
		if (newPosition > mLimit || newPosition < 0) throw IllegalArgumentException();
		mPosition = newPosition;
		if (mMark > mPosition) mMark = -1;
		return *this;
	}
	int limit() const { return mLimit; }
	Buffer& limit(int newLimit) {
		if (newLimit > mCapacity || newLimit < 0) throw IllegalArgumentException();
		mLimit = newLimit;
		if (mPosition > mLimit) mPosition = mLimit;
		if (mMark > mLimit) mMark = -1;
		return *this;
	}
	Buffer& mark() {
		mMark = mPosition;
		return *this;
	}
	Buffer& reset() {
		int m = mMark;
		if (m < 0) throw InvalidMarkException();
		mPosition = m;
		return *this;
	}
	Buffer& clear() {
		mPosition = 0;
		mLimit = mCapacity;
		mMark = -1;
		return *this;
	}
	Buffer& flip() {
		mLimit = mPosition;
		mPosition = 0;
		mMark = -1;
		return *this;
	}
	Buffer& rewind() {
		mPosition = 0;
		mMark = -1;
		return *this;
	}
	int remaining() const {
		return mLimit - mPosition;
	}
	boolean hasRemaining() const {
		return mPosition < mLimit;
	}
	virtual boolean isReadOnly() const = 0;
	virtual boolean hasArray() const = 0;
	virtual Object array() const = 0;
	virtual int arrayOffset() const = 0;
	virtual boolean isDirect() const = 0;
};

class CharBuffer : extends Buffer,
	implements Comparable<CharBuffer>, implements Appendable, implements CharSequence, implements Readable
{
private:
	Array<char> hb;
};

} //namespace nio

#endif
