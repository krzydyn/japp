#include <lang/String.hpp>
#include <lang/System.hpp>
#include <lang/Exception.hpp>

static void test_String() {
	String s1;
	String s2("abc");
	String s3(s2);
	String s4(1);
	String s5('x');
	String s6(1.0);

	String s7(null);
}

int main(int argc, const char *argv[]) {
	try {
		test_String();
	} catch(const lang::Exception& e) {
		e.printStackTrace();
	}
	return 0;
}
