#ifndef __IO_PRINTSTREAM_HPP
#define __IO_PRINTSTREAM_HPP

#include <lang/String.hpp>
#include <io/OutputStream.hpp>
#include <cstring>
#include <cstdarg>

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

	void flush() {TRACE;
		synchronized (*this) {
			out.flush();
		}
	}
	void close() {TRACE;
		synchronized (*this) {
			out.close();
		}
	}

	void write(int b) {TRACE;
		synchronized (*this) {
			out.write(b);
		}
	}
	void write(const char *buf, int off, int len) {TRACE;
		synchronized (*this) {
			out.write(buf, off, len);
		}
	}

	void print(boolean b) const {TRACE;
		write(b ? "true" : "false");
	}
	void print(char c) const {TRACE;
		write(String::valueOf(c));
	}
	void print(int i) const {TRACE;
		write(String::valueOf(i));
	}

    void print(long l) const {TRACE;
		write(String::valueOf(l));
	}
	void print(float f) const {TRACE;
		write(String::valueOf(f));
	}
	void print(double d) const {TRACE;
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
	template <class T>
	void print(const T& s) const {TRACE;
		if (instanceOf<const Object>(s)) {
			print(s.toString());
		}
		else {
			const void *ptr = (const void *)&s;
			print(Integer::toHexString((unsigned long)ptr));
		}
	}

	void println() {TRACE;
		newLine();
	}
	template <class T>
	void println(const T& s) const {TRACE;
		synchronized (*this) {
			print(s);
		   	newLine();
		}
	}

	const PrintStream& printf(const String& fmt...) const {
		va_list args;
		va_start(args, fmt);
		format(fmt.intern().c_str(),args);
		va_end(args);
		return *this;
	}
	const PrintStream& printf(const char *fmt...) const {
		va_list args;
		va_start(args, fmt);
		format(fmt,args);
		va_end(args);
		return *this;
	}

	const PrintStream& append(const CharSequence& csq) const {
		print(csq.toString());
		return *this;
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
	void format(const char *fmt, va_list& args) const {
	}
};

} //namespace io

#endif
