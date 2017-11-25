#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Object.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdarg>

namespace lang {

//TODO Locale
class Locale {
public:
	static Locale getDefault();
};

interface CharSequence : Interface {
public:
	virtual int length() const = 0;
	virtual char charAt(int index) const = 0;
	//virtual const CharSequence& subSequence(int start, int end) const = 0;
	virtual String toString() const = 0;
};

class String final : extends Object, implements CharSequence {
private:
	std::string value;
	long hash = 0;

	static const char * emptystr;
	static void copystr(String *d, const char *s);
	static void copy(String *d, const String *s);
	static void move(String *d, const String *s);

	static String valueHex(long);
	static String getTypeName(const std::type_info&);

public:
	String(String&& o) {move(this,&o);}
	String(const String& o) {copy(this,&o); }
	String& operator=(String&& o) {move(this,&o);return*this;}
	String& operator=(const String& o) {copy(this,&o);return*this;}
	String(const std::string& v) {value = v; }

	String(const char *v) {TRACE;copystr(this, v); }
	explicit String(const std::nullptr_t&) {TRACE;copystr(this, (const char *)0); }

	String() : value(emptystr) {}
	String(const String& s, int offset, int count);
	String(const char *s, int offset, int count);

	const std::string& intern() const {TRACE; return value; }

	int length() const { return value.length(); }
	boolean isEmpty() const { return value.length() == 0; }
	char charAt(int index) const;

	String operator+(const char *s) const {TRACE;
		return value+s;
	}
	String operator+(const std::string& s) const {TRACE;
		return value+s;
	}
	String operator+(const String& s) const {TRACE;
		return value+s.value;
	}
	String operator+(const Object& s) const {TRACE;
		return value+s.toString().intern();
	}
	template<class T>
	String operator+(const T& v) const {TRACE;
		return value+std::to_string(v);
	}
	String& operator+=(const char *rhs){TRACE;
		value += rhs;
		return *this;
	}
	String& operator+=(const String& rhs){TRACE;
		value += rhs.value;
		return *this;
	}
	boolean equals(const char *str) const {TRACE;
		return str==null ? value == "" : value == str;
	}
	boolean equals(const Object& anObject) const {TRACE;
		if (this == &anObject) return true;
		if (!instanceOf<String>(anObject)) return false;
		return value == ((const String&)anObject).value;
	}

	boolean startsWith(const String& prefix, int toffset) const {TRACE;
		int pc = prefix.value.length();
		if ((toffset < 0) || (toffset > length() - pc)) {
			return false;
		}
		for (int po = 0; --pc >= 0; ++toffset,++po) {
			if (value[toffset] != prefix.value[po]) return false;
		}
		return true;
	}
	boolean startsWith(const String& prefix) const {TRACE;
		return startsWith(prefix, 0);
	}
	boolean endsWith(const String& suffix) const {TRACE;
		return startsWith(suffix, value.length() - suffix.value.length());
	}

	jint hashCode() const {
		jint h = hash;
		if (h == 0 && value.length() > 0) {
			for (int i = 0; i < length(); i++) {
				h = 31 * h + value[i];
			}
		}
		return h;
	}
	jint hashCode() {
		jint h = hash;
		if (h == 0 && value.length() > 0) {
			h = ((const Object *)this)->hashCode();
			hash = h;
		}
		return h;
	}

	int indexOf(int ch, int fromIndex=0) const {TRACE; return value.find((char)ch, fromIndex); }
	int indexOf(const String& str, int fromIndex=0) const {TRACE; return value.find(str.value, fromIndex); }

	int lastIndexOf(int ch, int fromIndex=-1) const {TRACE;
	   return value.rfind((char)ch, fromIndex < 0 ? std::string::npos : fromIndex);
	}
	int lastIndexOf(const String& str, int fromIndex=-1) const {TRACE;
		return value.rfind(str.value, fromIndex < 0 ? std::string::npos : fromIndex);
	}

	String substring(int beginIndex) const {TRACE; return value.substr(beginIndex); }
	String substring(int beginIndex, int endIndex) const {TRACE; return value.substr(beginIndex,endIndex-beginIndex); }
	/*const CharSequence& subSequence(int beginIndex, int endIndex) const {
		return substring(beginIndex, endIndex);
	}*/
	String concat(String str) {TRACE; return *this + str; }
	String replace(char oldChar, char newChar) {TRACE;
		if (oldChar != newChar) {
			int len = value.length();
			int i = -1;
			const std::string& val = value;
			while (++i < len) {
				if (val[i] == oldChar) {
					break;
				}
			}
			if (i < len) {
				std::string buf(len, ' ');
				for (int j = 0; j < i; j++) {
					buf[j] = val[j];
				}
				while (i < len) {
					char c = val[i];
					buf[i] = (c == oldChar) ? newChar : c;
					i++;
				}
				return String(buf);
			}
		}
		return String(*this);
	}
	boolean matches(String regex) {TRACE;
		//TODO return Pattern.matches(regex, this);
		return false;
	}
	String toLowerCase(Locale locale) const {TRACE; return *this; }
	String toLowerCase() const {TRACE;
		return toLowerCase(Locale::getDefault());
	}
	String toUpperCase(Locale locale) const {TRACE; return *this; }
	String toUpperCase() const {TRACE; return *this; }
	String trim() const {TRACE; return *this; }
	String toString() const {
		return *this;
	}

	static String valueOf(boolean b) {TRACE;return b ? "true" : "false"; }
	static String valueOf(char n) {TRACE;return std::to_string(n); }
	static String valueOf(short n) {TRACE;return std::to_string(n); }
	static String valueOf(unsigned short n) {TRACE;return std::to_string(n); }
	static String valueOf(int n) {TRACE;return std::to_string(n); }
	static String valueOf(unsigned n) {TRACE;return std::to_string(n); }
	static String valueOf(long n) {TRACE;return std::to_string(n); }
	static String valueOf(unsigned long n) {TRACE;return std::to_string(n); }
	static String valueOf(float n) {TRACE;return std::to_string(n); }
	static String valueOf(double n) {TRACE;return std::to_string(n); }

	static String valueOf(const Object& obj) {TRACE; return obj.toString(); }
	static String valueOf(const void *ptr);
	static String valueOf(const char *s) {TRACE;return s;}
	static String valueOf(const String& s) {TRACE;return s;}
	template<class T>
	static String valueOf(const T& t) {TRACE;return getTypeName(typeid(t)) + "@" + valueHex((long)&t); }

	static String format(String fmt, ...) {TRACE;
		va_list args;
		va_start(args, fmt);
		String s = format(fmt.intern().c_str(), args);
		va_end(args);
		return s;
	}
	static String format(const char *fmt, ...) {TRACE;
		va_list args;
		va_start(args, fmt);
		String s = format(fmt, args);
		va_end(args);
		return s;
	}
	static String format(const char *fmt, va_list& args);
};

class StringBuilder : extends Object {
private:
	std::stringstream value;
public:
	StringBuilder() {}
	StringBuilder(int capacity) {
	}
	StringBuilder& append(char v) {TRACE;
		value << v;
		return *this;
	}
	StringBuilder& append(const char *str) {TRACE;
		value << str;
		return *this;
	}
	StringBuilder& append(const String& str) {TRACE;
		value << str.intern();
		return *this;
	}
	template<class T>
	StringBuilder& append(const T& t) {TRACE;
		value << String::valueOf(t).intern();
		return *this;
	}
	String toString() const {TRACE;
		return value.str();
	}
};

inline String operator+(const char *a,const String& s) {TRACE;
	return a + s.intern();
}
}

#endif
