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
	void print(unsigned long s) const {
		print(Integer::toHexString(s));
	}
	void print(int s) const {print((unsigned long)s);}

	void print(const String& s) const {TRACE;
		out.write(s.intern().c_str(),s.length());
	}
	void print(const Object* o) const {TRACE;
		print(o->toString());
	}

	template <class T>
	void print(const T& s) const {TRACE;
		if (instanceOf<const Object>(s)) {
			print(s.toString());
		}
		else {
			const Object *ptr = (const Object *)&s;
			print((unsigned long)ptr);
		}
	}

	template <class T>
	void println(const T& s) const {TRACE;
		print(s); println();
	}
};

} //namespace io

#endif
