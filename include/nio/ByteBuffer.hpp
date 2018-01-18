#ifndef __NIO_BYTEBUFER_HPP
#define __NIO_BYTEBUFER_HPP

//#include <lang/package.hpp>
#include <lang/Appendable.hpp>
#include <lang/Readable.hpp>
#include <nio/Buffer.hpp>

#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

namespace nio {

class ByteOrder {
private:
	int n;
	ByteOrder(int n) : n(n) {}
public:
	static const ByteOrder LITTLE_ENDIAN;
	static const ByteOrder BIG_ENDIAN;
	boolean operator==(const ByteOrder& o) { return n == o.n;}
	boolean operator!=(const ByteOrder& o) { return n != o.n;}
};

class ByteBuffer : extends Buffer, implements Comparable<ByteBuffer> {
private:
	boolean alloc=false;
protected:
	Array<byte>* hb;
	int mOffset;
	boolean mIsReadOnly = false;
	boolean bigEndian;

	ByteBuffer(int mark, int pos, int lim, int cap, int offset = 0) :
			Buffer(mark, pos, lim, cap), mOffset(offset) {
		hb = new Array<byte>(cap);
		alloc = true;
	}
	ByteBuffer(int mark, int pos, int lim, int cap, Array<byte>& hb, int offset = 0) :
			Buffer(mark, pos, lim, cap), mOffset(offset) {
		this->hb = &hb;
	}

	void move(ByteBuffer& o) {
		Buffer::move(o);
		hb = o.hb; o.hb=null;
		mOffset = o.mOffset; o.mOffset=0;
		mIsReadOnly = o.mIsReadOnly; o.mIsReadOnly=false;
	}

public:
	static Shared<ByteBuffer> allocateDirect(int capacity);
	static Shared<ByteBuffer> allocate(int capacity);
	static Shared<ByteBuffer> wrap(Array<byte>& array, int offset, int length);
	static Shared<ByteBuffer> wrap(Array<byte>& array) {
		return wrap(array, 0, array.length);
	}

	ByteBuffer(ByteBuffer&&o) {move(o);}
	ByteBuffer& operator=(ByteBuffer&&o) {move(o);return *this;}
	~ByteBuffer() {
		if (alloc) {
			delete hb;
		}
	}

	//virtual Shared<ByteBuffer> slice() const = 0;
	//virtual Shared<ByteBuffer> duplicate() const = 0;
	//virtual Shared<ByteBuffer> asReadOnlyBuffer() const = 0;
	boolean isReadOnly() const { return mIsReadOnly; }
	virtual byte get() = 0;
	virtual ByteBuffer& put(byte c) = 0;
	virtual byte get(int index) const = 0;
	virtual ByteBuffer& put(int index, byte c) = 0;
	virtual ByteBuffer& get(Array<byte>& dst, int offset, int length);
	virtual ByteBuffer& get(Array<byte>& dst) {
		return get(dst, 0, dst.length);
	}
	virtual ByteBuffer& put(ByteBuffer& src);
	virtual ByteBuffer& put(const Array<byte>& src, int offset, int length);
	virtual ByteBuffer& put(const Array<byte>& src) final {
		return put(src, 0, src.length);
	}
	virtual boolean hasArray() const final {
		return hb->length > 0 && !isReadOnly();
	}
	virtual Array<byte>& array() final {
		if (isReadOnly()) throw ReadOnlyBufferException();
		return *hb;
	}
	virtual int arrayOffset() const final {
		if (isReadOnly()) throw ReadOnlyBufferException();
		return mOffset;
	}
	virtual ByteBuffer& compact() = 0;
	virtual boolean isDirect() const = 0;
	virtual String toString() const;

	virtual jint hashCode() const;
	virtual boolean equals(const Object& ob) const;
	virtual int compareTo(const ByteBuffer& o) const;
	virtual ByteOrder order() final {
		return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
	}
	virtual ByteBuffer& order(ByteOrder bo) final {
		bigEndian = (bo == ByteOrder::BIG_ENDIAN);
		return *this;
	}
	/*
	virtual char getChar() = 0;
	virtual ByteBuffer& putChar(char value) = 0;
	virtual char getChar(int index) = 0;
	virtual ByteBuffer& putChar(int index, char value) = 0;
	virtual Shared<CharBuffer> asCharBuffer() = 0;
	virtual short getShort() = 0;
	virtual ByteBuffer& putShort(short value) = 0;
	virtual short getShort(int index) = 0;
	virtual ByteBuffer& putShort(int index,short value) = 0;
	//virtual Shared<ShortrBuffer> asShortBuffer() = 0;
	virtual int getInt() = 0;
	virtual ByteBuffer& putInt(int value) = 0;
	virtual int getInt(int index) = 0;
	virtual ByteBuffer& putInt(int index, int value) = 0;
	//virtual Shared<IntBuffer> asIntBuffer() = 0;
	virtual long getLong() = 0;
	virtual ByteBuffer& putLong(long value) = 0;
	virtual long getLong(int index) = 0;
	virtual ByteBuffer& putLong(int index, long value) = 0;
	//virtual Shared<LongBuffer> asLongBuffer() = 0;
	*/
};

} //namespace nio

#endif
