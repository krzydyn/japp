#ifndef __IO_WRITER_HPP
#define __IO_WRITER_HPP

#include <io/Appendable.hpp>
#include <io/Closeable.hpp>
#include <io/Flushable.hpp>

namespace io {

class Writer : extends Object, implements Appendable, implements Closeable, implements Flushable {
protected:
   	Object* lock;
	Writer() {lock=this;}
	Writer(Object* lock) {
		if (lock == null) throw NullPointerException();
		this->lock = lock;
	}
public:
	virtual void write(int c) {
		synchronized (lock) {
		}
	}
	virtual void write(const Array<char>& cbuf) {
		write(cbuf, 0, cbuf.length);
	}
	virtual void write(const Array<char>& cbuf, int off, int len)=0;
	virtual void write(const String& str) {
		write(str, 0, str.length());
	}
	virtual void write(const String& str, int off, int len) {
		synchronized (lock) {
		}
	}
	Writer& append(const CharSequence& csq) {
		write(csq.toString());
		return *this;
	}
	Writer& append(const CharSequence& csq, int start, int end) {
		return *this;
	}
};

} //namespace io

#endif
