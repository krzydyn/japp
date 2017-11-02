#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Object.hpp>
#include <string>
#include <iostream>
#include <sstream>

namespace lang {

class String final : public Object {
private:
	class AutoType;
	std::string value;
	long hash = 0;
	static void assign(String *d, const String *s);
	static void move(String *d, const String *s);
	static void assign(String *d, const char *s);
	static String valueOf(const AutoType& any);

public:
	String(String&& o) {TRACE;move(this,&o);}
	String(const String& o) {TRACE;assign(this,&o); }
	String& operator=(String&& o) {TRACE;move(this,&o);return*this;}
	String& operator=(const String& o) {TRACE; assign(this,&o);return*this;}

	String(const std::string& v) {TRACE; value = v; }
	String(const char *v) {TRACE; assign(this, v); }
	explicit String(const std::nullptr_t&) {TRACE; assign(this, (const char *)0); }
	template<class T>
	String(T v) {TRACE; value = std::to_string(v); }

	String() {}
	String(const String& s, int offset, int count);
	String(const char *s, int offset, int count);

	const std::string& intern() const { return value; }

	int length() const { return value.length(); }
	boolean isEmpty() const { return value.length() == 0; }
	char charAt(int index) const;

	virtual boolean operator==(const Object& s) const {TRACE; return equals(s); }
	virtual boolean operator!=(const Object& s) const {TRACE; return !equals(s); }
	virtual boolean operator==(const void *ptr) const {TRACE; return equals((const char *)ptr); }
	virtual boolean operator!=(const void *ptr) const {TRACE; return !equals((const char *)ptr); }

	String operator+(const char *s) const {TRACE;
		return value+s;
	}
	String operator+(const std::string& s) const {TRACE;
		return value+s;
	}
	String operator+(const String& s) const {TRACE;
		return value+s.value;
	}
	template<class T>
	String operator+(const T& v) const {TRACE;
		return value+std::to_string(v);
	}

	boolean equals(const char *str) const {TRACE;
		return str==null ? value == "" : value == str;
	}
	boolean equals(const Object& anObject) const {TRACE;
		if (this == &anObject) return true;
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

	long hashCode() const {TRACE;
		long h = hash;
		if (h == 0 && value.length() > 0) {
			for (int i = 0; i < length(); i++) {
				h = 31 * h + value[i];
			}
		}
		return h;
	}
	long hashCode() {TRACE;
		long h = hash;
		if (h == 0 && value.length() > 0) {
			h = ((const Object *)this)->hashCode();
			hash = h;
		}
		return h;
	}

	int indexOf(int ch, int fromIndex=0) const {TRACE; return value.find((char)ch, fromIndex); }
	int indexOf(const String& str, int fromIndex=0) const {TRACE; return value.find(str.value, fromIndex); }

	int lastIndexOf(const String& str, int fromIndex=0) const {TRACE; return value.rfind(str.value, fromIndex); }

	String substring(int beginIndex) const {TRACE; return value.substr(beginIndex); }
	String substring(int beginIndex, int endIndex) const {TRACE; return value.substr(beginIndex,endIndex-beginIndex); }

	static String valueOf(int n) {TRACE; return String(std::to_string(n)); }
	static String valueOf(unsigned n) {TRACE; return String(std::to_string(n)); }
	static String valueOf(const Object& obj) {TRACE; return obj.toString(); }
	template<class T>
	static String valueOf(const T& t) {TRACE; return valueOf((const AutoType&)t); }
};

inline String toString(const String& s) {TRACE;return s;}
template<class T>
inline String toString(T v) {TRACE;return String(v);}

class StringBuilder : public Object {
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
		return String(value.str());
	}
};

}

#endif
