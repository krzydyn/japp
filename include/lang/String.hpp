#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Object.hpp>
#include <string>

namespace lang {

class String final : public Object {
private:
	std::string value;
	long hash = 0;

public:
	String(String&& o) : value(std::move(o.value)) { }
	String(const std::string& s) : value(s) {}
	String& operator=(String&& o) = default;
	String& operator=(const String& o) = default;

	String() {}
	String(const String& s) : value(s.value) {}
	String(const char *s);
	String(const String& s, int offset, int count);
	const std::string& intern() const { return value; }

	int length() const { return value.length(); }
	boolean isEmpty() const { return value.length() == 0; }

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

	static String valueOf(const Object& obj) { return obj.toString(); }

	//friend String operator+(String s, const int& v) const {
	//	return String(s.value+v).c_str());
	//}
};

}


#endif
