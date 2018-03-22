#include <lang/Character.hpp>
#include <lang/Number.hpp>
#include <lang/System.hpp>

namespace {
char digit[] = "0123456789ABCDEFGHIJKLMNOPRSTUVWXYZ";
char getDigit(byte d) { return digit[d]; }
struct base {
   mutable std::ostream *_out;
   unsigned _base;

   base(unsigned value=10) : _base(value) {}

   const base& operator << (const unsigned long & data) const {
        switch(_base) {
            case 8: *_out << std::oct << data; break;
            case 10: *_out << std::dec << data; break;
            //case 16: *_out << std::hex << data; break;
            default: print(data); break;
        }
        return *this;
   }
   void print(unsigned long data) const {
        byte digits[8 * sizeof(data)], i = 0;
        while(data) {
             digits[i++] = (byte)(data % _base);
             data /= _base;
        }
        while(i) *_out << getDigit(digits[--i]);
   }
   friend const base& operator <<(std::ostream& out, const base& b)   {
       b._out = &out;
       return b;
   }
};
}

namespace lang {
const Boolean Boolean::TRUE(true);
const Boolean Boolean::FALSE(false);

boolean Boolean::equals(const Object& o) const {
	if (instanceof<Boolean>(&o)) {
		return value == ((const Boolean&)o).booleanValue();
	}
	return false;
}

String Boolean::toString(boolean v) {
	return v ? "true" : "false";
}

String Integer::toString(jint v, int radix) {
	if (radix <= 0) throw ArithmeticException();
	int s = 0;
	if (v < 0) {s=1;v=-v;}
	std::stringstream stream;
	stream << base((unsigned)radix) << (unsigned long)v;
	return s ? "-" + stream.str() : stream.str();
}
String Integer::toUnsignedString(jint v, int radix) {
	if (radix <= 0) throw ArithmeticException();
	std::stringstream stream;
	stream << base((unsigned)radix) << (unsigned long)v;
	return stream.str();
}

int Integer::parseInt(const String& s, int radix) {
	int result = 0;
	if (s.length() == 0) throw NumberFormatException("Empty input");
	boolean negative = false;
	int i=0;
	char c = s.charAt(0);
	if (c == '-') {negative=true; ++i;}
	else if (c == '+') {++i;}
	if (i == s.length()) throw NumberFormatException("Invalid input, no digits");
	for (; i < s.length(); ++i) {
		c = s.charAt(i);
		int digit = Character::digit(c, radix);
		if (digit < 0) throw NumberFormatException("Invalid input, wrong digit "+String::valueOf(c));
		result *= radix;
		result -= digit;
	}
	return negative ? result : -result;
}

String Long::toString(long v, int radix) {
	if (radix <= 0) throw ArithmeticException();
	int s = 0;
	if (v < 0) {s=1;v=-v;}
	std::stringstream stream;
	stream << base((unsigned)radix) << (unsigned long)v;
	return s ? "-" + stream.str() : stream.str();
}
String Long::toUnsignedString(long v, int radix) {
	if (radix <= 0) throw ArithmeticException();
	std::stringstream stream;
	stream << base((unsigned)radix) << (unsigned long)v;
	return stream.str();
}

} //namespace lang
