#ifndef __IO_WRITER_HPP
#define __IO_WRITER_HPP

#include <io/Appendable.hpp>
#include <io/Closeable.hpp>
#include <io/Flushable.hpp>

namespace io {

class Writer : extends Object, implements Appendable, implements Closeable, implements Flushable {
protected:
   	Object* lock;
	Writer() {lock = this;}
	Writer(Object* lock) {
		if (lock == null) throw NullPointerException();
		this->lock = lock;
	}
public:
	virtual void write(int c) {
		synchronized (*lock) {
			Array<char> a(1);
			a[0] = (char)c;
			write(a, 0, 1);
		}
	}
	virtual void write(const Array<char>& cbuf) {
		write(cbuf, 0, cbuf.length);
	}
	virtual void write(const Array<char>& cbuf, int off, int len) = 0;
	virtual void write(const String& str) {
		write(str, 0, str.length());
	}
	virtual void write(const String& str, int off, int len) {
		synchronized (*lock) {
			Array<char> cbuf(len);
			str.getChars(off, (off + len), &cbuf[0], 0);
			write(cbuf, 0, len);
		}
	}
	virtual Writer& append(const CharSequence& csq) {
		write(csq.toString());
		return *this;
	}
	virtual Writer& append(const CharSequence& csq, int start, int end) {
		write(csq.subSequence(start, end)->toString());
		return *this;
	}
	virtual Writer& append(char c) {
		write((int)c);
		return *this;
	}
	void flush() = 0;
	void close() = 0;
};

} //namespace io

#endif
