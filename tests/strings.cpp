#include <lang/String.hpp>

static void test_String() {
	String s1;
	String s2("abc");
	String s3(s2);

	String s4((char*)null);
}

int main(int argc, const char *argv[]) {
	test_String();
	return 0;
}
