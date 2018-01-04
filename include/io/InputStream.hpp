#ifndef __IO_INPUTSTREAM_HPP
#define __IO_INPUTSTREAM_HPP

#include <lang/Math.hpp>
#include <io/Closeable.hpp>
#include <io/IOException.hpp>

namespace io {

class InputStream : extends Object, implements Closeable {
public:
	static const int MAX_SKIP_BUFFER_SIZE = 2048;

	InputStream(const InputStream& o) = delete;
	InputStream& operator=(const InputStream& o) { return *this; }
	InputStream(const InputStream&& o) = delete;
	InputStream& operator=(InputStream&& o) { return *this; }

	InputStream() {}

	virtual int read() = 0;
	virtual int read(Array<byte>& cbuf, int off, int len) final {
		if (off >= cbuf.length) throw IndexOutOfBoundsException();
		if (off + len > cbuf.length) len = cbuf.length - off;
		return read(&cbuf[0], off, len);
	}
	virtual int read(void *b, int len) final {
		return read(b, 0, len);
	}
	virtual int read(void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return 0;
		int c = read();
		if (c == -1) return -1;
		((byte*)b)[off]=(byte)c;
		int i = 1;
		try {
			for (; i < len ; i++) {
				c = read();
				if (c == -1) break;
				((byte*)b)[off + i]=(byte)c;
			}
		} catch (const IOException&) {}
		return i;
	}
	virtual long skip(long n) {
		long remaining = n;
		int size = (int)Math.min(MAX_SKIP_BUFFER_SIZE, remaining);
		Array<byte> skipBuffer(size);
		while (remaining > 0) {
			int nr = read(skipBuffer, 0, (int)Math.min(size, remaining));
			if (nr < 0) break;
			remaining -= nr;
		}
		return n - remaining;
	}
	virtual int available() {return 0;}
	virtual void mark(int readlimit) {
		throw IOException("mark() not supported");
	}
	virtual void reset() {
		throw IOException("reset() not supported");
	}
	virtual boolean markSupported() {return false;}

	virtual void close() {}
};

} //namespace io

#endif
