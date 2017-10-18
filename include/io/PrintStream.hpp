#ifndef __IO_PRINTSTREAM_HPP
#define __IO_PRINTSTREAM_HPP

#include <lang/String.hpp>
#include <io/OutputStream.hpp>
#include <cstring>

namespace io {

class PrintStream : public OutputStream {
protected:
	 OutputStream& out;
public:
	//copy constructor
	PrintStream(const PrintStream& other) = delete;
	//move constructor
	PrintStream(PrintStream&& other) = delete;
	//copy assigment
	PrintStream& operator=(const PrintStream& other) {out=other.out; return *this;}
	//move assigment
	PrintStream& operator=(PrintStream&& other) = delete;

	PrintStream(OutputStream& o) : out(o) {}
	virtual ~PrintStream() {}

	void write(int b) {out.write(b);}

	void println() const {
		out.write('\n');
	}
	void print(const char *s) const {
		out.write(s,0,std::strlen(s));
	}
	void println(const char *s) const {
		out.write(s,0,std::strlen(s));
		out.write('\n');
	}

	void print(int s) const {
	}
	void println(int s) const {
	}

	void print(void *s) const {
		print(Integer::toHexString((unsigned long)s));
	}
	void println(void *s) const {
		println(Integer::toHexString((unsigned long)s));
	}

	void print(const String& s) const {
		out.write(s.intern().c_str(),s.length());
	}
	void println(const String& s) const {
		print(s);
		println();
	}
	void println(const Object& o) const {
		println(o.toString());
	}

	template <class T>
	void print(const T& s) const {
		print((void*)(&s));
	}
	template <class T>
	void println(const T& s) const {
		println((void*)(&s));
	}

};

} //namespace io

#endif
