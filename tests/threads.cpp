#include <lang/System.hpp>
#include <lang/Thread.hpp>

static void test_mainthread() {
	if (!Thread::currentThread().getName().equals("main"))
		System.err.println("main thread has wrong name");
}
static void test_backtrace() { TRACE;
	Thread::dumpStack();
}

static void test_thread() {
	static int x=1;
	class RunSleep1 : implements Runnable {
		void run() {
			x--;
			System.out.println("Sleeping for 1s");
			Thread::sleep(1000);
			int a=1/x;
			printf("%d\n",a);
		}
	} rs1;

	Thread t1(rs1, "threadX");
	t1.start();

	class RunSleep2 : extends Object, implements Runnable {
		void run() {
			System.out.println(Thread::currentThread().getName()+" sleeping for 10s");
			Thread::sleep(10000);
		}
	} rs2;
	Thread nt[10];
	for (int i=0; i < 10; ++i) {
		nt[i] = Thread(rs2);
	}
	for (int i=0; i < 10; ++i) {
		nt[i].start();
	}
}

int main(int argc, const char *argv[]) { TRACE;
	test_mainthread();
	test_backtrace();
	test_thread();
}
