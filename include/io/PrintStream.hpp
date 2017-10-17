#ifndef __IO_PRINTSTREAM_HPP
#define __IO_PRINTSTREAM_HPP

#include <lang/String.hpp>
#include <iostream>

namespace io {

class PrintStream {
private:
	//use pointer to impl. assign oparation while std::ostream assign is protected
	std::ostream* out;
public:
	//copy constructor
	PrintStream(const PrintStream& other) : out(other.out) {}
	//move constructor
	PrintStream(PrintStream&& other) { out=other.out; other.out=null; }
	//copy assigment
	PrintStream& operator=(const PrintStream& other) {out=other.out; return *this;}
	//move assigment
	PrintStream& operator=(PrintStream&& other) {
		if (this != &other) {out=other.out; other.out=null;}
		return *this;
	}
	virtual ~PrintStream() {}

	PrintStream(std::ostream& s) : out(&s) {}

	void println() const {
		std::cout << std::endl;
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
		std::cout << "PTR:" << std::to_string((unsigned long)s) << std::endl;
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
	void println(const T& s) const {
		println((void*)(&s));
	}

};

} //namespace io

#endif
