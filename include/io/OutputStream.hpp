#ifndef __IO_OUTPUTSTREAM_HPP
#define __IO_OUTPUTSTREAM_HPP

#include <io/Closeable.hpp>
#include <io/Flushable.hpp>
#include <lang/Exception.hpp>
#include <cstring> //std::strlen

namespace io {

class OutputStream : extends Object, implements Closeable, implements Flushable {
public:
	OutputStream(const OutputStream& o) = delete;
	OutputStream& operator=(const OutputStream& other) {return *this;}
	OutputStream(const OutputStream&& o) = delete;
	OutputStream& operator=(OutputStream&& other) = delete;

	OutputStream() {}

	virtual void write(int b) = 0;
	virtual void write(const void *b, int len) {
		write(b, 0, len);
	}
	virtual void write(const char *b) {
		if (b == null) throw NullPointerException();
		write(b, (int)std::strlen(b));
	}
	virtual void write(const void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return ;
		for (int i = 0 ; i < len ; i++) write((int)((char*)b)[off + i]);
	}
	virtual void write(const Array<byte>& b) final {
		write(&b[0], 0, b.length);
	}
	virtual void write(const Array<byte>& b, int off, int len) final {
		write(&b[0], off, b.length);
	}

	void flush() {}
	void close() {}
};

} //namespace io

#endif
