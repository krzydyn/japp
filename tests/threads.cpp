#include <ThreadPool.hpp>

static void test_backtrace() { TRACE;
	Exception e("Sample exception msg");
	e.printStackTrace();
}

int main(int argc, const char *argv[]) { TRACE;
	test_backtrace();
}
