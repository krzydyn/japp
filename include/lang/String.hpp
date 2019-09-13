#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Comparable.hpp>
#include <algorithm>
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace nio { namespace charset {
class Charset;
}}

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
	virtual Shared<CharSequence> subSequence(int start, int end) const = 0;
	virtual String toString() const = 0;
};

class String final : extends Object, implements CharSequence, implements Comparable<String> {
private:
	std::string value;
	int hash = 0;

	static const char * emptystr;
	static void copystr(String *d, const char *s);
	static void copy(String *d, const String *s);
	static void move(String *d, const String *s);

	static String valueHex(long);
	static String className(const std::type_info&);

	void init(const byte* s, int vlen, int offset, int count);
public:
	String(String&& o) {move(this,&o);}
	String(const String& o) {copy(this,&o); }
	String& operator=(String&& o) {move(this,&o);return*this;}
	String& operator=(const String& o) {copy(this,&o);return*this;}
	String(const std::string& v) {value = v; }

	String(const char *v) {copystr(this, v); }
	explicit String(const std::nullptr_t&) {copystr(this, (const char *)0); }

	String() : value(emptystr) {}
	String(const Array<char>& s) : String(s, 0, s.length) {}
	String(const Array<char>& s, int offset, int count);
	String(const Array<int>& cp, int offset, int count);
	String(const Array<byte>& s, const String& charsetName) : String(s, 0, s.length, charsetName) {}
	String(const Array<byte>& s, int offset, int count, const String& charsetName);
	String(const Array<byte>& s, const nio::charset::Charset& charset) : String(s, 0, s.length, charset) {}
	String(const Array<byte>& s, int offset, int count, const nio::charset::Charset& charset);
	String(const Array<byte>& s) : String(s, 0, s.length) {}
	String(const Array<byte>& s, int offset, int count);

	const std::string& intern() const { return value; }
	const char *cstr() const { return value.c_str(); }

	int length() const { return (int)value.length(); }
	boolean isEmpty() const { return value.length() == 0; }
	char charAt(int index) const;
	void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) const;
	Array<byte> getBytes() const {
		return Array<byte>((byte*)value.c_str(),(int)value.length());
	}
	Array<byte> getBytes(const String& charsetName) const;
	Array<byte> getBytes(const nio::charset::Charset& charset) const;

	String operator+(char c) const {
		return value+c;
	}
	String operator+(const char *s) const {
		if (s == null) return value+"<null>";
		return value+s;
	}
	String operator+(char *s) const {
		if (s == null) return value+"<null>";
		return value+s;
	}
	String operator+(const std::string& s) const {
		return value+s;
	}
	String operator+(const String& s) const {
		return value+s.value;
	}
	String operator+(const Object& s) const {
		return value+s.toString().intern();
	}
	template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	String operator+(const T& v) const {
		return value+std::to_string(v);
	}
	String& operator+=(char rhs){
		value += rhs;
		return *this;
	}
	String& operator+=(const char *rhs){
		if (rhs == null) value += "<null>";
		else value += rhs;
		return *this;
	}
	String& operator+=(const String& rhs){
		value += rhs.value;
		return *this;
	}
	boolean equals(const char *str) const {
		return str==null ? value == "" : value == str;
	}
	boolean equals(const String& o) const {
		if (this == &o) return true;
		return value == o.value;
	}
	boolean equals(const Object& o) const {
		if (this == &o) return true;
		if (!instanceof<String>(&o)) return false;
		return value == ((const String&)o).value;
	}
	boolean equalsIgnoreCase(const String& s) const {
		size_t sz = value.size();
		if (s.value.size() != sz) return false;
		for (size_t i=0; i < sz; ++i) {
			if (tolower(value[i]) != tolower(s.value[i])) return false;
		}
		return true;
	}

	boolean startsWith(const String& prefix, int toffset) const {
		int pc = (int)prefix.value.length();
		if ((toffset < 0) || (toffset > length() - pc)) {
			return false;
		}
		for (unsigned po = 0; --pc >= 0; ++toffset,++po) {
			if (value[(unsigned)toffset] != prefix.value[po]) return false;
		}
		return true;
	}
	boolean startsWith(const String& prefix) const {
		return startsWith(prefix, 0);
	}
	boolean endsWith(const String& suffix) const {
		return startsWith(suffix, (int)(value.length() - suffix.value.length()));
	}

	int hashCode() const {
		int h = hash;
		if (h == 0 && value.length() > 0) {
			for (int i = 0; i < length(); i++) {
				h = 31 * h + value[(unsigned)i];
			}
			const_cast<int&>(this->hash) = h;
		}
		return h;
	}

	int indexOf(int ch, int fromIndex=0) const { return (int)value.find((char)ch, (unsigned)fromIndex); }
	int indexOf(const String& str, int fromIndex=0) const { return (int)value.find(str.value, (unsigned)fromIndex); }

	int lastIndexOf(int ch, int fromIndex=-1) const {
	   return (int)value.rfind((char)ch, fromIndex < 0 ? std::string::npos : (unsigned)fromIndex);
	}
	int lastIndexOf(const String& str, int fromIndex=-1) const {
		return (int)value.rfind(str.value, fromIndex < 0 ? std::string::npos : (unsigned)fromIndex);
	}

	String substring(int beginIndex) const {
		return value.substr((unsigned)beginIndex);
	}
	String substring(int beginIndex, int endIndex) const {
		return value.substr((unsigned)beginIndex,(unsigned)(endIndex-beginIndex));
	}
	Shared<CharSequence> subSequence(int beginIndex, int endIndex) const {
		return makeShared<String>(substring(beginIndex, endIndex));
	}
	String concat(String str) { return *this + str; }
	String replace(char oldChar, char newChar) const {
		if (oldChar != newChar) {
			int len = (int)value.length();
			int i = -1;
			const std::string& val = value;
			while (++i < len) {
				if (val[(unsigned)i] == oldChar) {
					break;
				}
			}
			if (i < len) {
				std::string buf((unsigned)len, ' ');
				for (int j = 0; j < i; j++) {
					buf[(unsigned)j] = val[(unsigned)j];
				}
				while (i < len) {
					char c = val[(unsigned)i];
					buf[(unsigned)i] = (c == oldChar) ? newChar : c;
					i++;
				}
				return String(buf);
			}
		}
		return String(*this);
	}
	int compareTo(const String& anotherString) const {
		return value.compare(anotherString.value);
	}
	int compareToIgnoreCase(const String& str) const {
		return compareTo(str);
	}
	boolean matches(String regex) {
		//TODO return Pattern.matches(regex, this);
		return false;
	}
	boolean contains(const String& s) const {
		return indexOf(s) > -1;
	}
	String toLowerCase(Locale locale) const {
		std::string s(value);
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
   	}
	String toLowerCase() const {
		return toLowerCase(Locale::getDefault());
	}
	String toUpperCase(Locale locale) const {
		std::string s(value);
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
	}
	String toUpperCase() const {
		return toUpperCase(Locale::getDefault());
	}
	String trim() const { return *this; }
	String toString() const {
		return *this;
	}

	static String valueOf(boolean b) {return b ? "true" : "false"; }
	static String valueOf(char n) {return std::string(1,n); }
	static String valueOf(short n) {return std::to_string(n); }
	static String valueOf(unsigned short n) {return std::to_string(n); }
	static String valueOf(int n) {return std::to_string(n); }
	static String valueOf(unsigned n) {return std::to_string(n); }
	static String valueOf(long n) {return std::to_string(n); }
	static String valueOf(unsigned long n) {return std::to_string(n); }
	static String valueOf(long long n) {return std::to_string(n); }
	static String valueOf(float n) {return std::to_string(n); }
	static String valueOf(double n) {return std::to_string(n); }

	static String valueOf(const char *s) {return s;}
	static String valueOf(char *s) {return valueOf((const char*)s);}
	static String valueOf(const Object& obj) { return obj.toString(); }
	static String valueOf(const String& s) {return s;}
	static String valueOf(const std::thread::id& id) {
		std::stringstream s;
		s << id;
		return s.str();
	}
	template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	static String valueOf(const T& t) {return className(typeid(t)) + "@" + valueHex((long)&t); }

	static String format(String fmt, ...) {
		va_list args;
		va_start(args, fmt);
		String s = format(fmt.cstr(), args);
		va_end(args);
		return s;
	}
	static String format(const char *fmt, ...) {
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
	StringBuilder& append(char v) {
		value << v;
		return *this;
	}
	StringBuilder& append(const char *str) {
		value << str;
		return *this;
	}
	StringBuilder& append(const String& str) {
		value << str.intern();
		return *this;
	}
	template<class T>
	StringBuilder& append(const T& t) {
		value << String::valueOf(t).intern();
		return *this;
	}
	String toString() const {
		return value.str();
	}
};

inline String operator+(const char *a,const String& s) {
	return a + s.intern();
}
}

#endif
