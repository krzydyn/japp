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
	PrintStream(const PrintStream& o) = delete;
	PrintStream& operator=(const PrintStream& o) { out = o.out; return *this;}
	PrintStream(PrintStream&& o) = delete;
	PrintStream& operator=(PrintStream&& o) = delete;

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

	void print(const char *s) const {TRACE;
		if (s == null) s = "null";
		write(s);
	}
	void print(const Object& s) const {TRACE; write(s.toString());}
	void print(const String& s) const {TRACE; write(s);}
	template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	void print(const T& v) const {TRACE; write(String::valueOf(v)); }

	void println() {TRACE; newLine(); }
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
	void format(const char *fmt, va_list& args) const {
		write(String::format(fmt,args));
	}

	const PrintStream& append(const CharSequence& csq) const {
		write(csq.toString());
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
};

} //namespace io

#endif
