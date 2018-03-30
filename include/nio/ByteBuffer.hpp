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
	const boolean allocated;
protected:
	Array<byte>* hb = null;
	int mOffset;
	boolean mIsReadOnly = false;
	boolean bigEndian = true;

	ByteBuffer(int mark, int pos, int lim, int cap, int offset = 0) :
			Buffer(mark, pos, lim, cap), allocated(true), mOffset(offset) {
		hb = new Array<byte>(cap);
		for (int i=0; i < cap; ++i) (*hb)[i] = 0;
	}
	ByteBuffer(int mark, int pos, int lim, int cap, Array<byte>& hb, int offset = 0) :
			Buffer(mark, pos, lim, cap), allocated(false), mOffset(offset) {
		this->hb = &hb;
	}

	void move(ByteBuffer& o) {
		LOGD("ByteBuffer::move");
		Buffer::move(o);
		hb = o.hb; o.hb=null;
		const_cast<boolean&>(allocated) = o.allocated;
		mOffset = o.mOffset; o.mOffset=0;
		mIsReadOnly = o.mIsReadOnly; o.mIsReadOnly=false;
		bigEndian = o.bigEndian; o.bigEndian=true;
	}

public:
	static Shared<ByteBuffer> allocateDirect(int capacity);
	static Shared<ByteBuffer> allocate(int capacity);
	static Shared<ByteBuffer> wrap(Array<byte>& array, int offset, int length);
	static Shared<ByteBuffer> wrap(Array<byte>& array) {
		return wrap(array, 0, array.length);
	}

	ByteBuffer() : allocated(false) {}
	ByteBuffer(ByteBuffer&&o) : allocated(false) {move(o);}
	ByteBuffer& operator=(ByteBuffer&&o) {move(o);return *this;}
	~ByteBuffer() {
		if (allocated) delete hb;
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

	virtual jchar getChar() = 0;
	virtual ByteBuffer& putChar(jchar value) = 0;
	virtual jchar getChar(int index) const = 0;
	virtual ByteBuffer& putChar(int index, jchar value) = 0;
	//virtual Shared<CharBuffer> asCharBuffer() = 0;
	virtual short getShort() = 0;
	virtual ByteBuffer& putShort(short value) = 0;
	virtual short getShort(int index) const = 0;
	virtual ByteBuffer& putShort(int index,short value) = 0;
	//virtual Shared<ShortrBuffer> asShortBuffer() = 0;
	virtual int getInt() = 0;
	virtual ByteBuffer& putInt(int value) = 0;
	virtual int getInt(int index) const = 0;
	virtual ByteBuffer& putInt(int index, int value) = 0;
	//virtual Shared<IntBuffer> asIntBuffer() = 0;
	virtual jlong getLong() = 0;
	virtual ByteBuffer& putLong(jlong value) = 0;
	virtual jlong getLong(int index) const = 0;
	virtual ByteBuffer& putLong(int index, jlong value) = 0;
	//virtual Shared<LongBuffer> asLongBuffer() = 0;
};

} //namespace nio

#endif
