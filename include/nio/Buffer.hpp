#ifndef __NIO_BUFFER_HPP
#define __NIO_BUFFER_HPP

#include <lang/Exception.hpp>
#include <lang/System.hpp>

namespace nio {

class InvalidMarkException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class BufferOverflowException : extends RuntimeException {
	using RuntimeException::RuntimeException;
};

class BufferUnderflowException : extends RuntimeException {
	using RuntimeException::RuntimeException;
};

class ReadOnlyBufferException : extends UnsupportedOperationException {
	using UnsupportedOperationException::UnsupportedOperationException;
};

class Buffer : extends Object {
private:
	// mXxxxx = member Xxxxx
	int mMark = -1;
	int mPosition = 0;
	int mLimit;
	int mCapacity;

protected:
	void move(Buffer& o) {
		mMark = o.mMark; o.mMark=-1;
		mPosition = o.mPosition; o.mPosition=0;
		mLimit = o.mLimit; o.mLimit=0;
		mCapacity = o.mCapacity; o.mCapacity=0;
	}
	Buffer(){}
	Buffer(Buffer&&o) {move(o);}
	Buffer& operator=(Buffer&&o) {move(o);return *this;}
	Buffer(int mark, int pos, int lim, int cap) : mCapacity(cap) {
		if (cap < 0) throw IllegalArgumentException("Negative capacity: " + String::valueOf(cap));
		limit(lim);
		position(pos);
		if (mark >= 0) {
			if (mark > pos) throw IllegalArgumentException("mark > position");
			this->mMark = mark;
		}
	}

	int nextGetIndex() {
		if (mPosition >= mLimit) throw BufferUnderflowException();
		return mPosition++;
	}
	int nextGetIndex(int nb) {
		if (mLimit - mPosition < nb) throw BufferUnderflowException();
		int p = mPosition;
		mPosition += nb;
		return p;
	}

	int nextPutIndex() {
		if (mPosition >= mLimit) throw BufferOverflowException();
		return mPosition++;
	}
	int nextPutIndex(int nb) {
		if (mLimit - mPosition < nb) throw BufferOverflowException();
		int p = mPosition;
		mPosition += nb;
		return p;
	}

	int checkIndex(int i) const {
		if ((i < 0) || (i >= mLimit)) throw IndexOutOfBoundsException();
		return i;
	}
	int checkIndex(int i, int nb) const {
		if ((i < 0) || (nb > mLimit - i)) throw IndexOutOfBoundsException();
		return i;
	}
	int markValue() const {return mMark;}
	void truncate() {
		mMark = -1;
		mPosition = 0;
		mLimit = 0;
		mCapacity = 0;
	}
	void discardMark() {mMark = -1;}

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
	virtual Object& array() = 0;
	virtual int arrayOffset() const = 0;
	virtual boolean isDirect() const = 0;
};

} //namespace nio

#endif
