#include <lang/System.hpp>
#include <lang/Thread.hpp>

static void test_backtrace() { TRACE;
	Exception e("Sample exception msg");
	e.printStackTrace();
}

static void test_thread() {
	class TestRun : public Runnable {
		void run() {
			System.out.println("Sleeping");
			Thread::sleep(1000);
		}
	} tr;

	Thread t1(tr, "threadX");
	t1.start();
	//Thread t2("threadX",[=] {Thread::sleep(100);});
}

int main(int argc, const char *argv[]) { TRACE;
	test_backtrace();
	test_thread();
}
