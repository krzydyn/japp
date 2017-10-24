#include <ThreadPool.hpp>

static void test_backtrace() {
	Exception e;
	e.fillInStackTrace();
}

int main(int argc, const char *argv[]) {
	test_backtrace();
}
