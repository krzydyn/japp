#ifndef __LANG_MATH_HPP
#define __LANG_MATH_HPP

//TODO put into Math class as statics

inline int mmin(int a, int b) {return a<b?a:b;}
inline int mmax(int a, int b) {return a>b?a:b;}
inline unsigned long newton(unsigned n, unsigned k) {
	if (k > n) return 0;
	if (k==0 || n==k) return 1;
    if (k > n-k) k = n-k; // nCr(n,k) = nCr(n,n-k)
    
    unsigned long x=n,px;
	unsigned d=2;
    for (int i=1; i < k; ++i) {
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
inline int init_t8(unsigned char *b) {
    b[0]=0;
    for (int i=1; i < 256; ) {
        int i0=i; i<<=1;
        for (int j=0; j < i; ++j)
            b[i0+j]=b[j]+1;
    }
    return 1;
}
int init2_t8(unsigned char *b) {
    b[0]=0;
    for (int i=1; i < 256; ++i) {
        int j=i>>1;
        b[i]=b[i-j];
    }
}
inline unsigned bitCount_t8(unsigned i) {
    static unsigned char bits[256];
    static unsigned done=init_t8(bits);
    unsigned cnt=bits[i&0xff];
    i>>=8; cnt+=bits[i&0xff];
    i>>=8; cnt+=bits[i&0xff];
    i>>=8; cnt+=bits[i&0xff];
    return cnt;
}
inline unsigned bitMask(unsigned n) {
    return (1 << n) - 1;
}

#endif
