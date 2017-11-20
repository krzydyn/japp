#include <lang/Number.hpp>

namespace {
char digit[] = "0123456789ABCDEFGHIJKLMNOPRSTUVWXYZ";
char getDigit(int d) { return digit[d]; }
struct base {

   mutable std::ostream *_out;
   unsigned _value;

   base(unsigned value=10) : _value(value) {}

   const base& operator << (const long & data) const {
        switch(_value) {
            case 8: *_out << std::oct << data; break;
            case 10: *_out << std::dec << data; break;
            case 16: *_out << std::hex << data; break;
            //default: print(data); break;
        }
        return *this;
   }
   void print(unsigned long data) const {
        int digits[8 * sizeof(data)], i = 0;
        while(data) {
             digits[i++] = data % _value;  
             data /= _value;
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

String Integer::toUnsignedString(int v, int radix) {
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return stream.str();
}

String Long::toUnsignedString(long v, int radix) {
	unsigned long x = v;
	std::stringstream stream;
	stream << base(radix) << x;
	return stream.str();
}

} //namespace lang
