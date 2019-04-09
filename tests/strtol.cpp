#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct Test {
	int x;
	int arr1[0];
	int arr[];
};

int main() {
	int32_t x = INT32_MIN;
	uint32_t y;

	if (x == INT32_MIN) y = INT32_MAX;
	else if (x < 0) y = -x; else y = x;

	printf("x=%x(%d)   y=%x(%d)\n", x, x, y, y);

	printf("sizeof(Test) = %ld\n", sizeof(Test));
	return 0;
}
