#ifndef __IO_OUTPUTSTREAM_HPP
#define __IO_OUTPUTSTREAM_HPP

#include <lang/Exception.hpp>
#include <io/Closeable.hpp>
#include <iostream>

namespace io {

class OutputStream : public Object, public Closeable {
public:
	virtual void write(int b) = 0;
	virtual void write(const void *b, int len) {
		write(b, 0, len);
	}
	virtual void write(const void *b, int off, int len) {
		if (b == null) throw NullPointerException();
		if ((off < 0) || (len < 0) || ((off + len) < 0))
			throw IndexOutOfBoundsException();
		if (len == 0) return ;
		for (int i = 0 ; i < len ; i++) write((int)((char*)b)[off + i]);
	}

	void close() {}
};

} //namespace io

#endif
