#ifndef __IO_INPUTSTREAM_HPP
#define __IO_INPUTSTREAM_HPP

#include <io/Closeable.hpp>
#include <lang/Exception.hpp>

namespace io {

class InputStream : extends Object, implements Closeable {
public:
	InputStream& operator=(const InputStream& other) { return *this; }
	InputStream& operator=(InputStream&& other) { return *this; }

	virtual int read() = 0;
	virtual int read(const void *b, int len) {
		return read(b, 0, len);
	}
	virtual int read(const void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return 0;
		int i;
		for (i = 0 ; i < len ; i++) {
			int r = read();
			if (r < 0) break;
			((char*)b)[off + i]=r;
		}
		return i;
	}

	void flush() {}
	void close() {}
};

} //namespace io

#endif
