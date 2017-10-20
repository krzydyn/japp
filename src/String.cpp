#include <lang/System.hpp>
#include <lang/Exception.hpp>

namespace lang {

void String::assign(String *d, const String *s) {
	if (s == null || d == null) throw NullPointerException();
	std::cout << "assign '" << s->value << "'" << std::endl;
	d->value = s->value;
	d->hash = s->hash;
}
void String::move(String *d, const String *s) {
	if (s == null || d == null) throw NullPointerException();
	std::cout << "move '" << s->value << "'" << std::endl;
	d->value = std::move(s->value);
	d->hash = s->hash;
}
/*
void String::assign(String *d, const char *s) {
	if (s == null || d == null) throw NullPointerException();
	std::cout << "assign_cstr '" << s << "'" << std::endl;
	d->value = s;
	d->hash = 0;
}
*/
String::String(const String& s, int offset, int count) {
	if (&s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || s.value.size() - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = s.value.substr(offset, count);
}
String::String(const char *s, int offset, int count) {
	if (s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || strlen(s) - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = std::string(s).substr(offset, count);
}

char String::charAt(int index) {
	if ((index < 0) || (index >= value.length())) {
		throw IndexOutOfBoundsException(index);
	}
	return value[index];
}

}
