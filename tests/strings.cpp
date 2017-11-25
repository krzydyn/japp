#include <lang/String.hpp>
#include <lang/System.hpp>
#include <lang/Exception.hpp>

void test_String() {TRACE;
	String s1;
	String s2("abc");
	String s3(s2);
	String s4 = String::valueOf(1);
	String s5 = String::valueOf('x');
	String s6 = String::valueOf(1.0);

	try {
		String s7(null);
		throw Exception("ERR: expected exception");
	} catch(const NullPointerException& e) {
		System.out.println("OK: expected exception received");
		e.printStackTrace();
	}
}

int main(int argc, const char *argv[]) {TRACE;
	try {
		test_String();
	} catch(const lang::Exception& e) {
		e.printStackTrace();
	}
	return 0;
}
