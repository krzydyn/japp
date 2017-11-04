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
	PrintStream(const PrintStream& other) = delete;
	PrintStream(PrintStream&& other) = delete;
	PrintStream& operator=(const PrintStream& other) { out = other.out; return *this; }
	PrintStream& operator=(PrintStream&& other) { out = std::move(other.out); return *this; }

	PrintStream(OutputStream& o) : out(o) {}

	void flush() {
		synchronized (*this) {
			out.flush();
		}
	}
	void close() {
		synchronized (*this) {
			out.close();
		}
	}

	void write(int b) {
		synchronized (*this) {
			out.write(b);
		}
	}
	void write(void *buf, int off, int len) {
		synchronized (*this) {
			out.write(buf, off, len);
		}
	}

	void print(boolean b) const {TRACE;
		write(b ? "true" : "false");
	}
	void print(char c) const {
		write(String::valueOf(c));
	}
	void print(int i) const {
		write(String::valueOf(i));
	}

    void print(long l) const {
		write(String::valueOf(l));
	}
	void print(unsigned long l) const {TRACE;
		print(Integer::toHexString(l));
	}
	void print(float f) const {
		write(String::valueOf(f));
	}
	void print(double d) const {
		write(String::valueOf(d));
	}

	void print(const char *s) const {TRACE;
		if (s == null) {
			s = "null";
		}
		write(s);
	}
	void print(const String& s) const {TRACE;
		write(s);
	}
	void println() {
		newLine();
	}

	template <class T>
	void print(const T& s) const {TRACE;
		if (instanceOf<const Object>(s)) {
			print(s.toString());
		}
		else {
			const void *ptr = (const void *)&s;
			print((unsigned long)ptr);
		}
	}

	template <class T>
	void println(const T& s) const {TRACE;
		synchronized (*this) {
			print(s);
		   	newLine();
		}
	}

private:
	void newLine() const {
		synchronized (*this) {
			out.write('\n');
		}
	}
	void write(const char *s) const {
		synchronized (*this) {
			out.write(s,strlen(s));
		}
	}
	void write(const String& s) const {
		synchronized (*this) {
			out.write(s.intern().c_str(),s.length());
		}
	}
};

} //namespace io

#endif
