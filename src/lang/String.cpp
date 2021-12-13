#include <lang/Class.hpp>
#include <lang/Exception.hpp>
#include <lang/Number.hpp>
#include <lang/System.hpp>

namespace lang {

namespace {
	Locale defaultLocale;
}

Locale Locale::getDefault() {
	return defaultLocale;
}

const char *String::emptystr="";

void String::copystr(String *d, const char *s) {TRACE;
	if (s == null || d == null) throw NullPointerException();
	d->value = s;
	d->hash = 0;
}
void String::copy(String *d, const String *s) {TRACE;
	if (s == null || d == null) throw NullPointerException();
	d->value = s->value;
	d->hash = s->hash;
}
void String::move(String *d, const String *s) {TRACE;
	if (s == null || d == null) throw NullPointerException();
	if (s->intern().empty()) d->value=emptystr;
	else d->value = std::move(s->value);
	d->hash = s->hash;
}

void String::init(const byte* value, int vlen, int offset, int count) {
	if (offset < 0) throw IndexOutOfBoundsException(offset);
	if (count < 0) throw IndexOutOfBoundsException(count);
	if (vlen - count < offset) throw IndexOutOfBoundsException(offset + count);
	this->value = std::string((const char *)value, (unsigned)offset, (unsigned)count);
}
String::String(const Array<char>& value, int offset, int count) {TRACE;
	if (value == null) throw NullPointerException();
	init((const byte*)&value[0], value.length, offset, count);
}
String::String(const Array<byte>& value, int offset, int count) {TRACE;
	if (value == null) throw NullPointerException();
	init(&value[0], value.length, offset, count);
}

char String::charAt(int index) const {TRACE;
	if ((index < 0) || (index >= length())) {
		throw IndexOutOfBoundsException(index);
	}
	return value[(unsigned)index];
}

void String::getChars(int srcBegin, int srcEnd, char *dst, int dstBegin) const {
	if (srcBegin < 0) throw IndexOutOfBoundsException(srcBegin);
	if ((unsigned)srcEnd > value.length()) throw IndexOutOfBoundsException(srcEnd);
	if (srcBegin > srcEnd) throw IndexOutOfBoundsException(srcEnd - srcBegin);
	memcpy(dst + dstBegin, value.c_str() + srcBegin, srcEnd - srcBegin);
}

String String::valueHex(long l) {TRACE; return Long::toHexString(l); }

String String::format(const char *fmt, va_list& args) {TRACE;
	char buffer[1023];
	String s;
	if ((unsigned)vsnprintf(buffer,sizeof(buffer),fmt,args) > sizeof(buffer))
		strcpy(buffer+1023-5,"...");
	s = buffer;
	//FILE *stream;
	//vfprintf(stream, fmt, args);
	return s;
}

String String::className(const std::type_info& type) {
	return class(type).getName();
}

} //namespace lang
