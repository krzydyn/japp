#include <lang/Exceptions.hpp>

namespace lang {

String::String(const char *s) {
	if (s == null) throw NullPointerException();
	value = s;
}

String::String(const String& s, int offset, int count) {
	if (offset < 0 || count < 0 || value.size() - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = s.value.substr(offset, count);
}

}
