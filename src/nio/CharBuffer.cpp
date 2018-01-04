#include <lang/Math.hpp>
#include <nio/CharBuffer.hpp>

#include <lang/Comparable.hpp>
#include <lang/Readable.hpp>

namespace {
void checkBounds(int off, int len, int size) { 
	if ((off | len | (off + len) | (size - (off + len))) < 0)
		throw IndexOutOfBoundsException();
}
int static_compare(char x, char y) {
	//return Character.compare(x, y);
	return y - x;
}
int static_equals(char x, char y) {
	return x == y;
}
}

namespace nio {
class HeapCharBuffer : extends CharBuffer {
protected:
	int ix(int i) const {return i + mOffset;}
	String toString(int start, int end) const {
		return String(&hb[0], start + mOffset, end - start);
	}
public:
	HeapCharBuffer(int cap, int lim) : CharBuffer(-1, 0, lim, cap, 0) {}
	HeapCharBuffer(Array<char>& buf, int off, int len) : CharBuffer(-1, off, off + len, buf.length, buf, 0) {}

	char get() { return hb[ix(nextGetIndex())]; }
	char get(int i) const { return hb[ix(checkIndex(i))]; }

	boolean isDirect() const {return false;}
	boolean isReadOnly() const {return false;}

	CharBuffer& put(char x) {
		hb[ix(nextPutIndex())] = x;
		return *this;
	}
	CharBuffer& put(int i, char x) {
		hb[ix(checkIndex(i))] = x;
		return *this;
	}

	Shared<CharSequence> subSequence(int start, int end) const {
		return null;
	}
};


Shared<CharBuffer> CharBuffer::allocate(int capacity) {
	if (capacity < 0) throw IllegalArgumentException();
	return makeShared<HeapCharBuffer>(capacity, capacity);
}
Shared<CharBuffer> CharBuffer::wrap(Array<char>& array, int offset, int length) {
	return makeShared<HeapCharBuffer>(array, offset, length);
}

int CharBuffer::read(CharBuffer& target) {
	int targetRemaining = target.remaining();
	int thisRemaining = remaining();
	if (thisRemaining == 0) return -1;
	int n = Math.min(thisRemaining, targetRemaining);
	int l = limit();
	Finalize([&]{limit(l);});
	if (targetRemaining < thisRemaining) limit(position() + n);
	if (n > 0) target.put(*this);
	return n;
}
CharBuffer& CharBuffer::get(Array<char>& dst, int offset, int length) {
	checkBounds(offset, length, dst.length);
	if (length > remaining()) throw BufferUnderflowException();
	int end = offset + length;
	for (int i = offset; i < end; i++)
		dst[i] = get();
	return *this;
}
CharBuffer& CharBuffer::put(CharBuffer& src) {
	if (&src == this) throw IllegalArgumentException();
	if (isReadOnly()) throw ReadOnlyBufferException();
	int n = src.remaining();
	if (n > remaining()) throw BufferOverflowException();
	for (int i = 0; i < n; i++)
		put(src.get());
	return *this;
}
CharBuffer& CharBuffer::put(const Array<char>& src, int offset, int length) {
	checkBounds(offset, length, src.length);
	if (length > remaining()) throw BufferOverflowException();
	int end = offset + length;
	for (int i = offset; i < end; i++)
		put(src[i]);
	return *this;
}
CharBuffer& CharBuffer::put(const String& src, int start, int end) {
	checkBounds(start, end - start, src.length());
	if (isReadOnly()) throw ReadOnlyBufferException();
	if (end - start > remaining()) throw BufferOverflowException();
	for (int i = start; i < end; i++)
		put(src.charAt(i));
	return *this;
}
jint CharBuffer::hashCode() const {
	jint h = 1;
	int p = position();
	for (int i = limit() - 1; i >= p; i--)
		h = 31 * h + (int)get(i);
	return h;
}
boolean CharBuffer::equals(const Object& ob) const {
	if (this == &ob) return true;
	if (!instanceof<CharBuffer>(&ob)) return false;
	CharBuffer& that = (CharBuffer&)ob;
	if (remaining() != that.remaining()) return false;
	int p = position();
	for (int i = limit() - 1, j = that.limit() - 1; i >= p; i--, j--) {
		if (!static_equals(get(i), that.get(j))) return false;
	}
	return true;
}

int CharBuffer::compareTo(const CharBuffer& o) const {
	int n = position() + Math.min(remaining(), o.remaining());
	for (int i = position(), j = o.position(); i < n; i++, j++) {
		int cmp = static_compare(get(i), o.get(j));
		if (cmp != 0) return cmp;
	}
	return remaining() - o.remaining();
}

} //namespace nio
