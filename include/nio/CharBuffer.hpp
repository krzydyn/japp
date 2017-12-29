#ifndef __NIO_CHARBUFER_HPP
#define __NIO_CHARBUFER_HPP

//#include <lang/package.hpp>
#include <lang/Appendable.hpp>
#include <lang/Readable.hpp>
#include <nio/Buffer.hpp>

namespace nio {

class CharBuffer;
class RestoreLimit {
	int l;
	CharBuffer *buf;
public:
	RestoreLimit(int l, CharBuffer *b) : l(l), buf(b) {}
	~RestoreLimit();
};

class CharBuffer : extends Buffer,
	implements Comparable<CharBuffer>, implements Appendable, implements CharSequence, implements Readable
{
protected:
	Array<char> hb;
	int mOffset;
	boolean mIsReadOnly = false;

	CharBuffer(int mark, int pos, int lim, int cap, int offset) :
		Buffer(mark, pos, lim, cap), mOffset(offset) {
		hb = Array<char>(cap);
	}
	CharBuffer(int mark, int pos, int lim, int cap) : Buffer(mark, pos, lim, cap), mOffset(0) {
	}

	virtual String toString(int start, int end) const = 0;

public:
	static Shared<CharBuffer> allocate(int capacity);

	virtual int read(CharBuffer& target);
	//virtual Shared<CharBuffer> slice() const = 0;
	//virtual Shared<CharBuffer> duplicate() const = 0;
	//virtual Shared<CharBuffer> asReadOnlyBuffer() const = 0;
	boolean isReadOnly() const { return mIsReadOnly; }
	virtual char get() = 0;
	virtual CharBuffer& put(char c) = 0;
	virtual char get(int index) const = 0;
	virtual CharBuffer& put(int index, char c) = 0;
	virtual CharBuffer& get(Array<char>& dst, int offset, int length);
	virtual CharBuffer& get(Array<char>& dst) {
		return get(dst, 0, dst.length);
	}
	virtual CharBuffer& put(CharBuffer& src);
	virtual CharBuffer& put(const Array<char>& src, int offset, int length);
	virtual CharBuffer& put(const Array<char>& src) final {
		return put(src, 0, src.length);
	}
	virtual CharBuffer& put(const String& src, int start, int end);
	virtual CharBuffer& put(String src) {
		return put(src, 0, src.length());
	}
	virtual boolean hasArray() const {
		return hb.length > 0 && !isReadOnly();
	}
	virtual Array<char>& array() {
		if (isReadOnly()) throw ReadOnlyBufferException();
		return hb;
	}
	virtual int arrayOffset() const {
		if (isReadOnly()) throw ReadOnlyBufferException();
		return mOffset;
	}
	//public abstract CharBuffer compact();
	virtual boolean isDirect() const = 0;

	virtual jint hashCode() const;
	virtual boolean equals(const Object& ob) const;
	virtual int compareTo(const CharBuffer& o) const;

	virtual String toString() const {
		return toString(position(), limit());
	}
	virtual int length() const final { return remaining(); }
	virtual char charAt(int index) const final {
		return get(position() + checkIndex(index, 1));
	}

	//inherited from CharSequence
	//public abstract CharBuffer subSequence(int start, int end);
	
	virtual CharBuffer& append(const CharSequence& csq) {
		if (((Object&)csq) == null_obj) return put("null");
		return put(csq.toString());
	}
	virtual CharBuffer& append(const CharSequence& csq, int start, int end) {
		if (((Object&)csq) == null_obj) return put("null");
		return put(csq.subSequence(start, end)->toString());
	}
	virtual CharBuffer& append(char c) {
		return put(c);
	}
	//public abstract ByteOrder order();
	//public IntStream chars();
};

} //namespace nio

#endif
