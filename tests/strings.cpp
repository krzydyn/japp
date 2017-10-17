#include <lang/String.hpp>
#include <lang/System.hpp>
#include <lang/Exception.hpp>

static void test_String() {
	String s1;
	String s2("abc");
	String s3(s2);

	String s4(null);
}

int main(int argc, const char *argv[]) {
	try {
	test_String();
	} catch(const lang::Exception& e) {
		System.err.println(e.toString());
	}
	return 0;
}
