#ifndef __LANG_MATH_HPP
#define __LANG_MATH_HPP

#include <lang/Exception.hpp>
#include <cmath>
#include <random>
#undef min
#undef max

namespace lang {

class Math {
public:
	static constexpr double E = 2.7182818284590452354;
	static constexpr double PI = 3.14159265358979323846;

	static int abs(int a) { return a >= 0 ? a : -a; }
	static long abs(long a) { return a >= 0 ? a : -a; }
	static float abs(float a) { return a >= 0 ? a : -a; }
	static double abs(double a) { return a >= 0 ? a : -a; }

	static double sin(double a) { return std::sin(a); }
	static double cos(double a) { return std::cos(a); }
	static double tan(double a) { return std::tan(a); }
	static double asin(double a) { return std::asin(a); }
	static double acos(double a) { return std::acos(a); }
	static double atan(double a) { return std::atan(a); }
	inline static double toRadians(double angdeg) {
		return angdeg / 180.0 * PI;
	}
	inline static double toDegrees(double angrad) {
		return angrad * 180.0 / PI;
	}
	static double exp(double a) { return std::exp(a); }
	static double log(double a) { return std::log(a); }
	static double log10(double a) { return std::log10(a); }
	static double sqrt(double a) { return std::sqrt(a); }
	static double cbrt(double a) { return std::cbrt(a); }
	static double IEEEremainder(double x, double y) { return std::remainder(x, y); }
	static double ceil(double a) { return std::ceil(a); }
	static double floor(double a) { return std::floor(a); }
	static double rint(double a) { return 0; }
	static double atan2(double y, double x) { return std::atan2(y, x); }

	static double pow(double a, double b) { return std::pow(a, b); }
	static int round(float a) { return (int) std::floor (a + 0.5f); }
	static long round(double a) { return (long) std::floor(a + 0.5); }

	static double random() {
		std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
		return uniform_dist(randomEngine);
	}

	static int addExact(int x, int y) {
		int r = x + y;
		if (((x ^ r) & (y ^ r)) < 0) throw ArithmeticException("integer overflow");
		return r;
	}
	static long addExact(long x, long y) {
		long r = x + y;
		if (((x ^ r) & (y ^ r)) < 0) throw ArithmeticException("integer overflow");
		return r;
	}

	inline static long min(int a, long b) {
		return min((long)a, b);
	}
	inline static long min(long a, int b) {
		return min(a, (long)b);
	}

	//return value is faster then return const ref !!!
	template<class T>
	inline static const T min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	template<class T>
	inline static T max(const T& a, const T& b) {
		return a < b ? b : a;
	}

private:
	static std::default_random_engine randomEngine;
};

} //namespace lang

inline unsigned long newton(unsigned n, unsigned k) {
	if (k > n) return 0;
	if (k==0 || n==k) return 1;
	if (k > n-k) k = n-k; // nCr(n,k) = nCr(n,n-k)

	unsigned long x=n,px;
	unsigned d=2;
	for (unsigned i=1; i < k; ++i) {
		px=x;
		x*=n-i;
		if (x < px) return 0;
		while (d <= k && x%d == 0) { x/=d; ++d; }
	}
	while (d <= k) { x/=d; ++d; }
	return x;
}
inline unsigned long factorial(unsigned n) {
	unsigned r=1;
	while (n>1) {r*=n; --n;}
	return r;
}
inline unsigned long power(unsigned a,unsigned n) {
	unsigned r=1;
	while (n) {
		if (n&1) r*=a;
		a*=a;
		n>>=1;
	}
	return r;
}
inline unsigned bitCount(unsigned i) {
	i -= ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
inline unsigned loBit(unsigned i) {
	return i & -i;
}
inline unsigned hiBit(unsigned i) {
	i |= i >> 1; i |= i >> 2; i |= i >> 4; i |= i >> 8; i |= i >> 16;
	return i - (i >> 1);
}
inline unsigned pow2up(unsigned i) {
	--i; i |= i >> 1; i |= i >> 2; i |= i >> 4; i |= i >> 8; i |= i >> 16; ++i;
	return i;
}
inline unsigned hiBitNext(unsigned i) {
	i |= i >> 1; i |= i >> 2; i |= i >> 4; i |= i >> 8; i |= i >> 16;
	return i + 1;
}
inline unsigned hiBitPos(unsigned i) {
	unsigned r=0;
	if (i&0xffff0000) { i>>=16; r+=16; }
	if (i&0xff00) { i>>=8; r+=8; }
	if (i&0xf0) { i>>=4; r+=4; }
	if (i&0xc) { i>>=2; r+=2; }
	if (i&0x2) { i>>=1; r+=1; }
	return r+i;
}
inline unsigned bitCount_O1(unsigned i) {
	unsigned cnt=0;
	while (i) { i &= i-1; ++cnt; }
	return cnt;
}
inline unsigned char *init_t8() {
	static unsigned char bits[256];
	bits[0]=0;
	for (int i=1; i < 256; ) {
		int i0=i; i<<=1;
		for (int j=0; j < i; ++j)
			bits[i0+j]=(unsigned char)(bits[j]+1);
	}
	return bits;
}
inline int init2_t8(unsigned char *b) {
	b[0]=0;
	for (int i=1; i < 256; ++i) {
		int j=i>>1;
		b[i]=b[i-j];
	}
	return 0;
}
inline unsigned bitCount_t8(unsigned i) {
	static unsigned char *bits=init_t8();
	unsigned cnt=bits[i&0xff];
	i>>=8; cnt+=bits[i&0xff];
	i>>=8; cnt+=bits[i&0xff];
	i>>=8; cnt+=bits[i&0xff];
	return cnt;
}
inline unsigned bitMask(unsigned n) {
	return (unsigned)((1 << n) - 1);
}

#endif
