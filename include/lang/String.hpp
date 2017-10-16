#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Object.hpp>
#include <string>
#include <iostream>

namespace lang {

class String : public Object {
private:
	std::string value;
	long hash = 0;

public:
	String(String&& o) noexcept : value(std::move(o.value)) { }
	String& operator=(String&& o) = default;
	String& operator=(const String& o) = default;
	/*String& operator=(String&& o) {
		std::cout << "operator=(const String&&)" << std::endl;
	   	value = std::move(o.value);
   	}
	String& operator=(const String& o) {
		std::cout << "operator=(const String&) " << std::endl;
		std::cout << "    from " << o.value << std::endl;
	   	value = o.value.c_str();
		std::cout << "    assigned " << std::endl;
   	}*/

	String(const std::string& s) : value(s) {}

	String() {}
	String(const String& s) : value(s.value) {}
	String(const char *s);
	String(const String& s, int offset, int count);
	const std::string& intern() const { return value; }

	int length() { return value.length(); }
	boolean isEmpty() { return value.length() == 0; }

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

	boolean equals(const Object& anObject) {
		if (this == &anObject) return true;
		return value == ((const String&)anObject).value;
	}

	boolean startsWith(const String& prefix, int toffset) {
		int pc = prefix.value.length();
		if ((toffset < 0) || (toffset > value.length() - pc)) {
			return false;
		}
		for (int po = 0; --pc >= 0; ++toffset,++po) {
			if (value[toffset] != prefix.value[po]) return false;
		}
		return true;
	}
	boolean startsWith(const String& prefix) {
		return startsWith(prefix, 0);
	}
	boolean endsWith(const String& suffix) {
		return startsWith(suffix, value.length() - suffix.value.length());
	}

	long hashCode() {
		long h = hash;
		if (h == 0 && value.length() > 0) {
			for (int i = 0; i < value.length(); i++) {
				h = 31 * h + value[i];
			}
			hash = h;
		}
		return h;
	}

	int indexOf(int ch) { return value.find((char)ch, 0); }
	int indexOf(int ch, int fromIndex) { return value.find((char)ch, fromIndex); }
	int indexOf(const String& str) { return value.find(str.value, 0); }
	int indexOf(const String& str, int fromIndex) { return value.find(str.value, fromIndex); }

	int lastIndexOf(const String& str) { return value.rfind(str.value); }
	int lastIndexOf(const String& str, int fromIndex) { return value.rfind(str.value, fromIndex); }

	String substring(int beginIndex) { return value.substr(beginIndex); }

	static String valueOf(const Object& obj) { return obj.toString(); }

	//friend String operator+(String s, const int& v) const {
	//	return String(s.value+v).c_str());
	//}
};

}


#endif
