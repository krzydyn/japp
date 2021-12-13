#include <lang/Exception.hpp>
#include <lang/String.hpp>
#include <lang/System.hpp>
#include <lang/Thread.hpp>

void test_formatString() {TRACE;
	String s = String::format("%d %.2f", 10, 1.1);
	System::out.println(s);
	System::out.printf("format %04d\n", 5);
}
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
		System::out.println("OK: expected exception received");
		e.printStackTrace();
	}
}

void test_SIGFAULT() {TRACE;
	char *p = nullptr;
	p[5] = 1;
}

int main(int argc, const char *argv[]) {TRACE;
	System::out.println(Thread::currentThread().getName());
	test_formatString();
	try {
		test_String();
		test_SIGFAULT();
	} catch(const lang::Exception& e) {
		e.printStackTrace();
	}
	return 0;
}
