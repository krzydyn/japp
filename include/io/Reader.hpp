#ifndef __IO_READER_HPP
#define __IO_READER_HPP

#include <io/Readable.hpp>
#include <io/Closeable.hpp>

namespace io {

class Reader : extends Object, implements Readable, implements Closeable {
protected:
	Object *lock;
	Reader() { lock=this; }
	Reader(Object* lock) {
		if (lock == null) throw NullPointerException();
		this->lock = lock;
	}

public:
	int read(nio::CharBuffer& target) {
		/*
		int len = target.remaining();
		char[] cbuf = new char[len];
		int n = read(cbuf, 0, len);
		if (n > 0)
			target.put(cbuf, 0, n);
		return n;*/
		return 0;
	}
};

} //namespace io
#endif
