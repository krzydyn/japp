#ifndef __LANG_NUMBER_HPP
#define __LANG_NUMBER_HPP

#include <lang/Comparable.hpp>
#include <lang/Exception.hpp>

namespace lang {

class NumberFormatException : extends Exception {
public:
	using Exception::Exception;
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

class Boolean : extends Object, implements Comparable<Boolean> {
private:
	boolean value;
public:
	static const Boolean TRUE;
	static const Boolean FALSE;

	static String toString(boolean v);
	static Boolean valueOf(boolean b) {return b ? TRUE : FALSE;}
	static Boolean valueOf(const String& s) {return parseBoolean(s) ? TRUE : FALSE;}
	static boolean parseBoolean(const String& s) {return s.equalsIgnoreCase("true");}
	static jint hashCode(boolean value) {return value ? 1231 : 1237;}
	static int compare(boolean x, boolean y) {return (x == y) ? 0 : (x ? 1 : -1);}

	Boolean(const Boolean& o) : value(o.value) {}
	Boolean(boolean value) : value(value) {}
	Boolean(const String& s) : value(parseBoolean(s)) {}
	boolean booleanValue() const {return value;}
	String toString() const { return toString(value); }
	jint hashCode() const {return Boolean::hashCode(value);}
	boolean equals(const Object& o) const;
	int compareTo(const Boolean& b) const { return compare(value,b.value); }
	operator int() const { return value?1:0; }
};

class Integer : extends Number, implements Comparable<Integer> {
private:
	int value;
public:
	static const int MIN_VALUE = (int)0x80000000;
	static const int MAX_VALUE = 0x7fffffff;

	static String toString(jint i, int radix=10);
	static String toUnsignedString(jint i, int radix=10);
	static String toHexString(jint i) {return toUnsignedString(i, 16);}
	static String toOctalString(jint i) {return toUnsignedString(i, 8);}
	static String toBinaryString(jint i) {return toUnsignedString(i, 2);}
	static int parseInt(const String& s, int radix=10);
	static int parseUnsignedInt(const String& s, int radix=10);
	static Integer valueOf(const String& s, int radix) {return Integer::valueOf(parseInt(s,radix));}
	static Integer valueOf(const String& s) {return Integer::valueOf(parseInt(s, 10));}
	static Integer valueOf(int i) {return Integer(i);}
	static Integer decode(String nm);
	static jint hashCode(int value) {return (jint)value;}
	static int compare(int x, int y) {return (x < y) ? -1 : ((x == y) ? 0 : 1);}

	Integer(const Integer& o) : value(o.value) {}
	Integer(int value) : value(value) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return (short)value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	String toString() const { return toString(value); }
	boolean equals(const Object& o) const {
		if (instanceof<Integer>(&o)) {
			return value == ((const Integer&)o).shortValue();
		}
		return false;
	}
	int compareTo(const Integer& b) const { return compare(value,b.value); }
	operator int() const { return value; }

	Integer operator+(int v) const {TRACE; return value+v;}
	Integer operator+(Integer v) const {TRACE; return value+v.value;}
	Integer& operator+=(int v) {TRACE; value+=v; return *this;}
	Integer& operator+=(Integer v) {TRACE; value+=v.value; return *this;}

};

class Byte : extends Number, implements Comparable<Byte> {
private:
	byte value;
public:
	static const int SIZE = 8;
	static const byte MIN_VALUE = -128;
	static const byte MAX_VALUE = 127;
	static String toString(byte s) { return Integer::toString((int)s, 10); }
	static Byte valueOf(byte b) { return Byte(b); }
	static byte parseByte(const String& s, int radix=10) {
		int i = Integer::parseInt(s, radix);
		if (i < MIN_VALUE || i > MAX_VALUE)
			throw NumberFormatException("Value out of range. Value:\"" + s + "\" Radix:" + radix);
		return (byte)i;
	}
	static jint hashCode(byte value) {return (jint)value;}
	static int compare(byte x, byte y) {return x - y;}

	Byte(const Byte& o) : value(o.value) {}
	Byte(byte value) : value(value) {}
	Byte(String s) : value(parseByte(s, 10)) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return (short)value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	String toString() const { return toString(value); }
	jint hashCode() const { return Byte::hashCode(value); }
	boolean equals(const Object& o) const {
		if (instanceof<Byte>(&o)) {
			return value == ((const Byte&)o).byteValue();
		}
		return false;
	}
	int compareTo(const Byte& b) const { return compare(value,b.value); }
	operator byte() const { return value; }
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
	static int compare(short x, short y) {return (x < y) ? -1 : ((x == y) ? 0 : 1);}

	Short(const Short& o) : value(o.value) {}
	Short(short value) : value(value) {}
	Short(String s) : value(parseShort(s, 10)) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return (short)value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	String toString() const { return toString(value); }
	jint hashCode() const { return Short::hashCode(value); }
	boolean equals(const Object& o) const {
		if (instanceof<Short>(&o)) {
			return value == ((const Short&)o).shortValue();
		}
		return false;
	}
	int compareTo(const Short& b) const { return compare(value,b.value); }
	operator short() const { return value; }

	Short operator+(short v) const {TRACE; return (short)(value+v);}
	Short operator+(Short v) const {TRACE; return (short)(value+v.shortValue());}
	Short& operator+=(short v) {TRACE; value=(short)(value+v); return *this;}
	Short& operator+=(Short v) {TRACE; value=(short)(value+v.shortValue()); return *this;}

	Integer operator+(int v) const {TRACE; return value+v;}
	Integer operator+(Integer v) const {TRACE; return value+v.intValue();}
};

class Long : extends Number, implements Comparable<Long> {
private:
	long value;
public:
	static const long MIN_VALUE = (long)0x8000000000000000L;
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
	static jint hashCode(long value) { return (jint)(value ^ (value >> 32)); }
	static int compare(long x, long y) {return (x < y) ? -1 : ((x == y) ? 0 : 1);}

	Long(const Long& o) : value(o.value) {}
	Long(long value) : value(value) {}
	byte byteValue() const {return (byte)value;}
	short shortValue() const {return (short)value;}
	int intValue() const {return (int)value;}
	long longValue() const {return (long)value;}
	float floatValue() const {return (float)value;}
	double doubleValue() const {return (double)value;}
	String toString() const { return toString(value); }
	jint hashCode() const { return Long::hashCode(value); }
	boolean equals(const Object& o) const {
		if (instanceof<Long>(&o)) {
			return value == ((const Long&)o).longValue();
		}
		return false;
	}
	int compareTo(const Long& b) const { return compare(value,b.value); }
	operator long() const { return value; }

	Long operator+(long v) const {TRACE; return value+v;}
	Long operator+(Long v) const {TRACE; return value+v.value;}
	Long& operator+=(long v) {TRACE; value+=v; return *this;}
	Long& operator+=(Long v) {TRACE; value+=v.value; return *this;}
};

} //namespace lang

#endif
