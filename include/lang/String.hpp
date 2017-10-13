#ifndef __LANG_STRING_HPP
#define __LANG_STRING_HPP

#include <lang/Exceptions.hpp>

namespace lang {

class String {
private:
	std::string value;

public:
	String() {}
	String(const char *s) {
		if (s == null) throw NullPointerException();
		value = s;
	}
	String(const String& s) {
		value = s.value;
	}
	String(const String& s, int offset, int count) {
		if (offset < 0 || count < 0 || value.size() - count < offset) {
            throw IndexOutOfBoundsException(offset);
        }
		value = s.value.substr(offset, count);
	}
};

}

#endif
