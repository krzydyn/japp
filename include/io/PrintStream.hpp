#ifndef __IO_PRINTSTREAM_HPP
#define __IO_PRINTSTREAM_HPP

#include <lang/String.hpp>
#include <iostream>

namespace io {

class PrintStream {
private:
	std::ostream& out;
public:
	PrintStream(std::ostream& s) : out(s) {}

	void print(String s) const {
		std::cout << s.intern();
	}
	void print(int s) const {
		std::cout << s;
	}
	void println(String s) const {
		std::cout << s.intern() << std::endl;
	}
	void println(int s) const {
		std::cout << s << std::endl;
	}
	void println(void *s) const {
		std::cout << std::to_string((unsigned long)s) << std::endl;
	}
	void println() const {
		std::cout << std::endl;
	}
};

} //namespace io

#endif
