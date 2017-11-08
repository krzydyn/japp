#ifndef __IO_OUTPUTSTREAM_HPP
#define __IO_OUTPUTSTREAM_HPP

#include <io/Closeable.hpp>
#include <io/Flushable.hpp>
#include <lang/Exception.hpp>

namespace io {

class OutputStream : extends Object, implements Closeable, implements Flushable {
public:
	OutputStream& operator=(const OutputStream& other) { return *this; }
	OutputStream& operator=(OutputStream&& other) { return *this; }

	virtual void write(int b) = 0;
	virtual void write(const void *b, int len) {
		write(b, 0, len);
	}
	virtual void write(const char *b) {
		if (b == null) throw NullPointerException();
		write(b, std::strlen(b));
	}
	virtual void write(const void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0)) throw IndexOutOfBoundsException();
		if (len == 0) return ;
		for (int i = 0 ; i < len ; i++) write((int)((char*)b)[off + i]);
	}

	void flush() {}
	void close() {}
};

} //namespace io

#endif
