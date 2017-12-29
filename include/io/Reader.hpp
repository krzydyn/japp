#ifndef __IO_READER_HPP
#define __IO_READER_HPP

#include <io/Closeable.hpp>
#include <lang/Readable.hpp>
#include <nio/CharBuffer.hpp>

namespace io {

class Reader : extends Object, implements Readable, implements Closeable {
private:
	static const int maxSkipBufferSize = 8192;
	Array<char> skipBuffer;
protected:
	Object *lock;
	Reader() { lock=this; }
	Reader(Object* lock) {
		if (lock == null) throw NullPointerException();
		this->lock = lock;
	}

public:
	int read(nio::CharBuffer& target) {
		int len = target.remaining();
		Array<char> cbuf(len);
		int n = read(cbuf, 0, len);
		if (n > 0)
			target.put(cbuf, 0, n);
		return n;
	}
	virtual int read() {
		Array<char> ch(1);
		if (read(ch, 0, 1) == -1)
			return -1;
		return ch[0];
	}
	virtual int read(Array<char>& cbuf) {
		return read(cbuf, 0, cbuf.length);
	}
	virtual int read(Array<char>& cbuf, int off, int len) = 0;
	virtual long skip(long n) {
		if (n < 0L) throw IllegalArgumentException("skip value is negative");
		int nn = (int) Math.min(n, (long)maxSkipBufferSize);
		long r = n;
		synchronized (lock) {
			if (skipBuffer.length < nn)
				skipBuffer = Array<char>(nn);
			while (r > 0) {
				int nc = read(skipBuffer, 0, (int)Math.min(r, (long)nn));
				if (nc == -1) break;
				r -= nc;
			}
		}
		return n - r;
	}
	virtual boolean ready() {
		return false;
	}
	virtual boolean markSupported() {
		return false;
	}
	virtual void mark(int readAheadLimit) {
		throw IOException("mark() not supported");
	}
	virtual void reset() {
		throw IOException("reset() not supported");
	}
};

} //namespace io
#endif
