#ifndef __LANG_NUMBER_HPP
#define __LANG_NUMBER_HPP

#include <lang/Exception.hpp>

namespace lang {

class NumberFormatException : extends Exception {
public:
	using Exception::Exception;
};

template<class T>
interface Comparable : Interface {
};

class Number : extends Object {
private:
	//static const jlong serialVersionUID = -8742448824652078965L;
public:
	virtual byte byteValue() const { return (byte)intValue(); }
	virtual short shortValue() const { return (short)intValue(); }
	virtual int intValue() const = 0;
	virtual long longValue() const = 0;
	virtual float floatValue() const = 0;
	virtual double doubleValue() const = 0;
};

class Boolean : implements Comparable<Boolean> {
private:
	boolean value;
public:
	Boolean(boolean value) : value(value) {}
};

class Integer : extends Number, implements Comparable<Integer> {
private:
	int value;
public:
	static const int MIN_VALUE = 0x80000000;
	static const int MAX_VALUE = 0x7fffffff;
	static String toString(int i, int radix=10);
	static String toUnsignedString(int i, int radix=10);
	static String toHexString(int i) {return toUnsignedString(i, 16);}
	static String toOctalString(int i) {return toUnsignedString(i, 8);}
	static String toBinaryString(int i) {return toUnsignedString(i, 2);}
	static int parseInt(const String& s, int radix=10);
	static int parseUnsignedInt(const String& s, int radix=10);
	static Integer valueOf(const String& s, int radix) {return Integer::valueOf(parseInt(s,radix));}
	static Integer valueOf(const String& s) {return Integer::valueOf(parseInt(s, 10));}
	static Integer valueOf(int i) {return Integer(i);}
	static Integer decode(String nm);
	static jint hashCode(int value) {return value;}

	Integer(Integer&& o) : value(o.value) {}
	Integer(int value) : value(value) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}

	operator int() { return value; }
};

class Short : extends Number, implements Comparable<Short> {
private:
	short value;
public:
	static const short MIN_VALUE = -32768;
	static const short MAX_VALUE = 32767;
	static String toString(short s) { return Integer::toString((int)s, 10); }
	static short parseShort(const String& s, int radix=10) {
		int i = Integer::parseInt(s, radix);
		if (i < MIN_VALUE || i > MAX_VALUE)
			throw NumberFormatException("Value out of range. Value:\"" + s + "\" Radix:" + radix);
		return (short)i;
	}
	static Short valueOf(const String& s, int radix=10) {
		return valueOf(parseShort(s, radix));
	}
	static Short valueOf(short s) { return Short(s); }
	static Short decode(const String& nm) {
		int i = Integer::decode(nm);
		if (i < MIN_VALUE || i > MAX_VALUE)
			throw NumberFormatException("Value " + String::valueOf(i) + " out of range from input " + nm);
		return valueOf((short)i);
	}
	static jint hashCode(short value) {return (jint)value;}

	Short(Short&& o) : value(o.value) {}
	Short(short value) : value(value) {}
	Short(String s) : value(parseShort(s, 10)) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	jint hashCode() const { return Short::hashCode(value); }
	boolean equals(const Object& o) const {
		if (instanceOf<Short>(&o)) {
			return value == ((const Short&)o).shortValue();
		}
		return false;
	}
};

class Long : extends Number, implements Comparable<Long> {
private:
	long value;
public:
	static const long MIN_VALUE = 0x8000000000000000L;
	static const long MAX_VALUE = 0x7fffffffffffffffL;
	static String toString(long i, int radix=10);
	static String toUnsignedString(long i, int radix=10);
	static String toHexString(long i) {return toUnsignedString(i, 16);}
	static String toOctalString(long i) {return toUnsignedString(i, 8);}
	static String toBinaryString(long i) {return toUnsignedString(i, 2);}
	static long parseLong(const String& s, int radix=10);
	static long parseUnsignedLong(const String& s, int radix=10);
	static Long valueOf(const String& s, int radix=10) {return Long::valueOf(parseLong(s, radix));}
	static Long valueOf(long l) {return Long(l);}
	static Long decode(const String& nm);
	static jint hashCode(long value) { return (int)(value ^ (value >> 32)); }
	

	Long(Long&& o) : value(o.value) {}
	Long(long value) : value(value) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	jint hashCode() const { return Long::hashCode(value); }
};

} //namespace lang

#endif
