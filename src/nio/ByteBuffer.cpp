#include <lang/Class.hpp>
#include <lang/Math.hpp>
#include <lang/Number.hpp>
#include <lang/String.hpp>
#include <nio/ByteBuffer.hpp>

namespace {
void checkBounds(int off, int len, int size) {
	if ((off | len | (off + len) | (size - (off + len))) < 0)
		throw IndexOutOfBoundsException();
}
int static_compare(byte x, byte y) {
	return Byte::compare(x, y);
}
int static_equals(byte x, byte y) {
	return x == y;
}
}

namespace nio {
class HeapByteBuffer : extends ByteBuffer {
protected:
	int ix(int i) const {return i + mOffset;}
public:
	HeapByteBuffer(int cap, int lim) : ByteBuffer(-1, 0, lim, cap, 0) {}
	HeapByteBuffer(Array<byte>& buf, int off, int len) : ByteBuffer(-1, off, off + len, buf.length, buf, 0) {}

	boolean isDirect() const {return false;}
	boolean isReadOnly() const {return false;}

	ByteBuffer& compact() {
		System.arraycopy(*hb, ix(position()), *hb, ix(0), remaining());
		position(remaining());
		limit(capacity());
		discardMark();
		return *this;
	}

	byte get() { return (*hb)[ix(nextGetIndex())]; }
	byte get(int i) const { return (*hb)[ix(checkIndex(i))]; }

	ByteBuffer& put(byte x) {
		(*hb)[ix(nextPutIndex())] = x;
		return *this;
	}
	ByteBuffer& put(int i, byte x) {
		(*hb)[ix(checkIndex(i))] = x;
		return *this;
	}

	jchar getChar() {
		int i = ix(nextGetIndex(2));
		return (jchar)(((*hb)[i] << 8) | (*hb)[i+1]);
	}
	ByteBuffer& putChar(jchar value) {
		int i = ix(nextPutIndex(2));
		(*hb)[i] = (byte)(value >> 8);
		(*hb)[i+1] = (byte)value;
		return *this;
	}
	jchar getChar(int index) const {
		int i = ix(checkIndex(index, 2));
		return (jchar)(((*hb)[i] << 8) | (*hb)[i+1]);
	}
	ByteBuffer& putChar(int index, jchar value) {
		int i = ix(checkIndex(index, 2));
		(*hb)[i] = (byte)(value >> 8);
		(*hb)[i+1] = (byte)value;
		return *this;
	}
	//virtual Shared<CharBuffer> asCharBuffer() = 0;
	short getShort() {
		int i = ix(nextGetIndex(2));
		return (short)(((*hb)[i] << 8) | (*hb)[i+1]);
	}
	ByteBuffer& putShort(short value) {
		int i = ix(nextPutIndex(2));
		(*hb)[i] = (byte)(value >> 8);
		(*hb)[i+1] = (byte)value;
		return *this;
	}
	short getShort(int index) const {
		int i = ix(checkIndex(index, 2));
		return (short)(((*hb)[i] << 8) | (*hb)[i+1]);
	}
	ByteBuffer& putShort(int index, short value) {
		int i = ix(checkIndex(index, 2));
		(*hb)[i] = (byte)(value >> 8);
		(*hb)[i+1] = (byte)value;
		return *this;
	}
	//virtual Shared<ShortrBuffer> asShortBuffer() = 0;
	int getInt() {
		int i = ix(nextGetIndex(4));
		return ((*hb)[i] << 24) | ((*hb)[i+1] << 16) | ((*hb)[i+2] << 8) | (*hb)[i+3];
	}
	ByteBuffer& putInt(int value) {
		int i = ix(nextPutIndex(4));
		(*hb)[i] = (byte)(value >> 24);
		(*hb)[i+1] = (byte)(value >> 16);
		(*hb)[i+2] = (byte)(value >> 8);
		(*hb)[i+3] = (byte)value;
		return *this;
	}
	int getInt(int index) const {
		int i = ix(checkIndex(index, 4));
		return ((*hb)[i] << 24) | ((*hb)[i+1] << 16) | ((*hb)[i+2] << 8) | (*hb)[i+3];
	}
	ByteBuffer& putInt(int index, int value) {
		int i = ix(checkIndex(index, 4));
		(*hb)[i] = (byte)(value >> 24);
		(*hb)[i+1] = (byte)(value >> 16);
		(*hb)[i+2] = (byte)(value >> 8);
		(*hb)[i+3] = (byte)value;
		return *this;
	}
	//virtual Shared<IntBuffer> asIntBuffer() = 0;
	jlong getLong() {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	ByteBuffer& putLong(jlong value) {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	jlong getLong(int index) const {
		int i = ix(checkIndex(index, 8));
		jlong l = 0;
		for (int j=0; j < 8; ++j) {
			l <<= 8;
			l |= (*hb)[i+j];
		}
		return l;
	}
	ByteBuffer& putLong(int index, jlong value) {
		int i = ix(checkIndex(index, 8));
		(*hb)[i+0] = (byte)(value >> 56);
		(*hb)[i+1] = (byte)(value >> 48);
		(*hb)[i+2] = (byte)(value >> 40);
		(*hb)[i+3] = (byte)(value >> 32);
		(*hb)[i+4] = (byte)(value >> 24);
		(*hb)[i+5] = (byte)(value >> 16);
		(*hb)[i+6] = (byte)(value >> 8);
		(*hb)[i+7] = (byte)value;
		return *this;
	}
	//virtual Shared<LongBuffer> asLongBuffer() = 0;
};

const ByteOrder ByteOrder::LITTLE_ENDIAN = ByteOrder(0);
const ByteOrder ByteOrder::BIG_ENDIAN = ByteOrder(1);

Shared<ByteBuffer> ByteBuffer::allocateDirect(int capacity) {
	return allocate(capacity);
}
Shared<ByteBuffer> ByteBuffer::allocate(int capacity) {
	if (capacity < 0) throw IllegalArgumentException();
	return makeShared<HeapByteBuffer>(capacity, capacity);
}
Shared<ByteBuffer> ByteBuffer::wrap(Array<byte>& array, int offset, int length) {
	return makeShared<HeapByteBuffer>(array, offset, length);
}

String ByteBuffer::toString() const {
	StringBuilder sb;
	sb.append(getClass().getName());
	sb.append("[pos=");
	sb.append(position());
	sb.append(" lim=");
	sb.append(limit());
	sb.append(" cap=");
	sb.append(capacity());
	sb.append("]");
	return sb.toString();
}

ByteBuffer& ByteBuffer::get(Array<byte>& dst, int offset, int length) {
	checkBounds(offset, length, dst.length);
	if (length > remaining()) throw BufferUnderflowException();
	int end = offset + length;
	for (int i = offset; i < end; i++)
		dst[i] = get();
	return *this;
}
ByteBuffer& ByteBuffer::put(ByteBuffer& src) {
	if (&src == this) throw IllegalArgumentException();
	if (isReadOnly()) throw ReadOnlyBufferException();
	int n = src.remaining();
	if (n > remaining()) throw BufferOverflowException();
	for (int i = 0; i < n; i++)
		put(src.get());
	return *this;
}
ByteBuffer& ByteBuffer::put(const Array<byte>& src, int offset, int length) {
	checkBounds(offset, length, src.length);
	if (length > remaining()) throw BufferOverflowException();
	int end = offset + length;
	for (int i = offset; i < end; i++)
		put(src[i]);
	return *this;
}
jint ByteBuffer::hashCode() const {
	jint h = 1;
	int p = position();
	for (int i = limit() - 1; i >= p; i--)
		h = 31 * h + (int)get(i);
	return h;
}
boolean ByteBuffer::equals(const Object& ob) const {
	if (this == &ob) return true;
	if (!instanceof<ByteBuffer>(&ob)) return false;
	ByteBuffer& that = (ByteBuffer&)ob;
	if (remaining() != that.remaining()) return false;
	int p = position();
	for (int i = limit() - 1, j = that.limit() - 1; i >= p; i--, j--) {
		if (!static_equals(get(i), that.get(j))) return false;
	}
	return true;
}

int ByteBuffer::compareTo(const ByteBuffer& o) const {
	int n = position() + Math::min(remaining(), o.remaining());
	for (int i = position(), j = o.position(); i < n; i++, j++) {
		int cmp = static_compare(get(i), o.get(j));
		if (cmp != 0) return cmp;
	}
	return remaining() - o.remaining();
}

} //namespace nio
