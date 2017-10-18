#ifndef __IO_PRINTSTREAM_HPP
#define __IO_PRINTSTREAM_HPP

#include <lang/String.hpp>
#include <io/OutputStream.hpp>

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
		std::cout << s;
	}
	void println(const char *s) const {
		std::cout << s << std::endl;
	}

	void print(int s) const {
		std::cout << s;
	}
	void println(int s) const {
		std::cout << s << std::endl;
	}

	void print(void *s) const {
		std::cout << "PTR:" << Integer::toHexString((unsigned long)s).intern();
	}
	void println(void *s) const {
		std::cout << "PTR:" << Integer::toHexString((unsigned long)s).intern() << std::endl;
	}

	void print(const String& s) const {
		std::cout << s.intern();
	}
	void println(const String& s) const {
		std::cout << s.intern() << std::endl;
	}
	void println(const Object& o) const {
		std::cout << o.toString().intern() << std::endl;
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
