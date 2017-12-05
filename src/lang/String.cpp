#include <lang/Class.hpp>
#include <lang/Exception.hpp>
#include <lang/Number.hpp>
#include <lang/System.hpp>

namespace lang {

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

String::String(const String& s, int offset, int count) {TRACE;
	if (&s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || s.length() - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = s.value.substr((unsigned)offset, (unsigned)count);
}
String::String(const char *s, int offset, int count) {TRACE;
	if (s == null || this == null) throw NullPointerException();
	if (offset < 0 || count < 0 || (int)strlen(s) - count < offset) {
		throw IndexOutOfBoundsException(offset);
	}
	value = std::string(s, (unsigned)offset, (unsigned)count);
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

String String::valueOf(const void *ptr) {TRACE; return Long::toHexString((unsigned long)ptr);}
String String::valueHex(long l) {TRACE; return Long::toHexString(l); }

String String::format(const char *fmt, va_list& args) {TRACE;
	static Object sync;
	static char buffer[1023];
	String s;
	synchronized (sync) {
		if ((unsigned)vsnprintf(buffer,sizeof(buffer),fmt,args) > sizeof(buffer)) {
			strcpy(buffer+1023-5,"...");
		}
		s = buffer;
	}
	//FILE *stream;
	//vfprintf(stream, fmt, args);
	return s;
}

String String::className(const std::type_info& type) {
	return class(type).getName();
}

} //namespace lang
