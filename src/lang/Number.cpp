#include <lang/Number.hpp>

namespace {
char digit[] = "0123456789ABCDEFGHIJKLMNOPRSTUVWXYZ";
char getDigit(int d) { return digit[d]; }
struct base {

   mutable std::ostream *_out;
   unsigned _base;

   base(unsigned value=10) : _base(value) {}

   const base& operator << (const long & data) const {
        switch(_base) {
            case 8: *_out << std::oct << data; break;
            case 10: *_out << std::dec << data; break;
            case 16: *_out << std::hex << data; break;
            default: print(data); break;
        }
        return *this;
   }
   void print(unsigned long data) const {
        int digits[8 * sizeof(data)], i = 0;
        while(data) {
             digits[i++] = data % _base;
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

String Integer::toString(int v, int radix) {
	int s = v >= 0 ? 0 : 1;
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return s ? "-" + stream.str() : stream.str();
}
String Integer::toUnsignedString(int v, int radix) {
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return stream.str();
}

String Long::toString(long v, int radix) {
	int s = v >= 0 ? 0 : 1;
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return s ? "-" + stream.str() : stream.str();
}
String Long::toUnsignedString(long v, int radix) {
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return stream.str();
}

} //namespace lang
