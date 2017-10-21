#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Object.hpp>
#include <string>
#include <iostream>
#include <sstream>

namespace lang {

class String final : public Object {
private:
	std::string value;
	long hash = 0;
	static void assign(String *d, const String *s);
	static void move(String *d, const String *s);
	static void assign(String *d, const char *s);
public:
	String(String&& o) { move(this,&o); }
	String(const String& o) { assign(this,&o); }
	String& operator=(String&& o) { move(this,&o); }
	String& operator=(const String& o) { assign(this,&o); }

	String(const std::string& v) { value = v; }
	String(const char *v) { assign(this, v); }
	explicit String(const std::nullptr_t&) { assign(this, (const char *)0); }
	template<class T>
	String(T v) { value = std::to_string(v); }

	String() {}
	String(const String& s, int offset, int count);
	String(const char *s, int offset, int count);

	const std::string& intern() const { return value; }

	int length() const { return value.length(); }
	boolean isEmpty() const { return value.length() == 0; }
	char charAt(int index);

	virtual boolean operator==(const Object& s) const { return equals(s); }
	virtual boolean operator!=(const Object& s) const { return !equals(s); }
	virtual boolean operator==(const void *ptr) const { return equals((const char *)ptr); }
	virtual boolean operator!=(const void *ptr) const { return !equals((const char *)ptr); }

	String operator+(const char *s) const {
		return value+s;
	}
	String operator+(const String& s) const {
		return value+s.value;
	}
	template<class T>
	String operator+(const T& v) const {
		return value+std::to_string(v);
	}

	boolean equals(const char *str) const {
		return str==null ? value == "" : value == str;
	}
	boolean equals(const Object& anObject) const {
		if (this == &anObject) return true;
		return value == ((const String&)anObject).value;
	}

	boolean startsWith(const String& prefix, int toffset) const {
		int pc = prefix.value.length();
		if ((toffset < 0) || (toffset > value.length() - pc)) {
			return false;
		}
		for (int po = 0; --pc >= 0; ++toffset,++po) {
			if (value[toffset] != prefix.value[po]) return false;
		}
		return true;
	}
	boolean startsWith(const String& prefix) const {
		return startsWith(prefix, 0);
	}
	boolean endsWith(const String& suffix) const {
		return startsWith(suffix, value.length() - suffix.value.length());
	}

	long hashCode() const {
		long h = hash;
		if (h == 0 && value.length() > 0) {
			for (int i = 0; i < value.length(); i++) {
				h = 31 * h + value[i];
			}
		}
		return h;
	}
	long hashCode() {
		long h = hash;
		if (h == 0 && value.length() > 0) {
			h = ((const Object *)this)->hashCode();
			hash = h;
		}
		return h;
	}

	int indexOf(int ch) const { return value.find((char)ch, 0); }
	int indexOf(int ch, int fromIndex) const { return value.find((char)ch, fromIndex); }
	int indexOf(const String& str) const { return value.find(str.value, 0); }
	int indexOf(const String& str, int fromIndex) const { return value.find(str.value, fromIndex); }

	int lastIndexOf(const String& str) const { return value.rfind(str.value); }
	int lastIndexOf(const String& str, int fromIndex) const { return value.rfind(str.value, fromIndex); }

	String substring(int beginIndex) const { return value.substr(beginIndex); }
	String substring(int beginIndex, int endIndex) { return value.substr(beginIndex,endIndex-beginIndex); }

	static String valueOf(int n) { return String(std::to_string(n)); }
	static String valueOf(unsigned n) { return String(std::to_string(n)); }
	static String valueOf(const Object& obj) { return obj.toString(); }
	template<class T>
	static String valueOf(const T& t) { return String(typeid(t).name()) + "@" + Integer::toHexString((long)&t); }
};

inline String toString(const String& s) {return s;}
template<class T>
inline String toString(T v) {return String(v);}

class StringBuilder : public Object {
private:
	std::stringstream value;
public:
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
		value << typeid(t).name() << "@" << Integer::toHexString((long)&t).intern();
		return *this;
	}
	String toString() const {
		return String(value.str());
	}
};

}

#endif
