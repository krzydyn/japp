#include <ThreadPool.hpp>

static void test_backtrace() {
	TRACE;
	Exception e;
	e.fillInStackTrace();
}

int main(int argc, const char *argv[]) {
	TRACE;
	test_backtrace();
}
