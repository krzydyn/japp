//#include "gp_adaptation_types.h"

#include <string.h>
#include <stdio.h>

static size_t tlv_getTL(const unsigned char *buf, unsigned *tag, size_t *len) {
	size_t i = 0;
	if (buf[i] == 0) return 0;
	//while (buf[i] == 0) ++i;
	*tag = buf[i++];
	if (((*tag) & 0x1f) == 0x1f) {
		do {
			*tag <<= 8; *tag |= buf[i]; ++i;
		} while (buf[i]&0x80);
	}
	*len = buf[i++];
	if (*len & 0x80) {
		int nb = (*len) & 0x7f;
		*len = 0;
		do {
			*len <<= 8; *len |= buf[i]; ++i;
			--nb;
		} while (nb > 0);
	}
	return i;
}
static unsigned tlv_get(const unsigned char *buf, size_t *lenptr, const unsigned char **valptr) {
	unsigned tag;
	size_t ret, len;
	ret = tlv_getTL(buf, &tag, &len);
	if (ret == 0) return 0;
	*valptr = buf + ret;
	*lenptr = len;
	return tag;
}
int strbin(const char *str, unsigned char *buf) {
	unsigned bt, j = 0;
	int bc=0;
	for (int i=0; str[i]; ++i) {
		char c = str[i];
		int bx;
		if (c >= '0' && c <= '9') bx = c-'0';
		else if (c >= 'a' && c <= 'f') bx = c-'a'+10;
		else if (c >= 'A' && c <= 'F') bx = c-'A'+10;
		else continue;
		bt <<= 4; bt |= bx;
		++bc;
		if (bc == 2) {
			buf[j++] = (unsigned char)bt;
			bc = bt = 0;
		}
	}
	return j;
}
void prhex(const char * l, const unsigned char *buf, size_t len) {
	printf("%s", l);
	for (unsigned i = 0; i < len; ++i)
		printf("%02X", buf[i]&0xff);
	printf("\n");
}

//304402202c78c2e09723cab4d9c668cf3213411ec4dd94a6122bf492e1812ea2108bca020220adcf4c2e606e099e58a5f5a5597334c2fc5dfbe715121cb7b971b5609855499f
int main(int argc, const char *argv[]) {
	unsigned char buf[1000];
	for (int i = 1; i < argc; ++i) {
		strbin(argv[i], buf);
		unsigned tag;
		size_t len;
		const unsigned char *val, *p;
		p = buf;
		tag = tlv_get(p, &len, &val);
		printf("Tag %X, len = %lu\n", tag, len);
		p = val;
		tag = tlv_get(p, &len, &val);
		printf("Tag %X, len = %lu\n", tag, len);
		prhex("val = ", val, len);
		p = val + len;
		tag = tlv_get(p, &len, &val);
		printf("Tag %X, len = %lu\n", tag, len);
		prhex("val = ", val, len);
	}
}
