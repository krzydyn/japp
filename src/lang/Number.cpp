#include <lang/Number.hpp>

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
