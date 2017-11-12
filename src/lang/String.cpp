#include <lang/Class.hpp>
#include <lang/Exception.hpp>
#include <lang/System.hpp>

namespace lang {

const char *String::emptystr="";

void String::copystr(String *d, const char *s) {
	if (s == null || d == null) throw NullPointerException();
	//std::cout << "copy_cstr '" << s << "'" << std::endl;
	d->value = s;
	d->hash = 0;
}
void String::copy(String *d, const String *s) {
	if (s == null || d == null) throw NullPointerException();
	//std::cout << "copy '" << s->value << "'" << std::endl;
	d->value = s->value;
	d->hash = s->hash;
}
void String::move(String *d, const String *s) {
	if (s == null || d == null) throw NullPointerException();
	//std::cout << "move '" << s->value << "'" << std::endl;
	if (s->intern().empty()) d->value=emptystr;
	else d->value = std::move(s->value);
	d->hash = s->hash;
}

String::String(const String& s, int offset, int count) {TRACE;
	if (&s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || s.length() - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = s.value.substr(offset, count);
}
String::String(const char *s, int offset, int count) {TRACE;
	if (s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || (int)strlen(s) - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = std::string(s, offset, count);
}

char String::charAt(int index) const {TRACE;
	if ((index < 0) || (index >= length())) {
		throw IndexOutOfBoundsException(index);
	}
	return value[index];
}

class String::AutoType {};
String String::valueOf(const String::AutoType& t) { return Class::nameOf(t) + "@" + Integer::toHexString((long)&t).intern(); }

} //namespace lang
