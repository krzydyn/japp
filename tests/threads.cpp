#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <lang/ThreadGroup.hpp>

void test_mainthread() {TRACE;
	System::out.println("Main thread name is "+Thread::currentThread().getName());
	if (!Thread::currentThread().getName().equals("main"))
		System::err.println("main thread has wrong name");
}
void test_backtrace() {TRACE;
	Thread::dumpStack();
}

void test_thread() {TRACE;
	class RunSleep1 : implements Runnable {
		void run() {TRACE;
			int x=0;
			System::out.println("Sleeping for 1s");
			Thread::sleep(1000);
			int a=100/x;
			System::out.printf("%d\n",a);
		}
	} rs1;

	System::out.println("Multi threading test");
	Thread t1(rs1, "threadX");
	t1.start();
	System::out.printf("waiting %s end\n", t1.getName().cstr());
	t1.join();
	System::out.println("threadX ended");

	class RunSleep2 : extends Object, implements Runnable {
		void run() {TRACE;
			System::out.println(Thread::currentThread().getName()+" sleeping for 10s");
			Thread::sleep(5000);
		}
	} rs2;
	Thread nt[10];
	for (int i=0; i < 10; ++i) {
		nt[i] = Thread(rs2);
	}
	for (int i=0; i < 10; ++i) {
		nt[i].start();
	}

	try{
		((Runnable&)rs1).run();
	}catch(const Throwable& e) {
		e.printStackTrace();
	}

	Thread([]{
			System::out.println("inline func");
	}).start();
}

int main(int argc, const char *argv[]) {TRACE;
	test_mainthread();
	test_backtrace();
	test_thread();
	System::out.println("Threads done");
	Thread::sleep(1000);
}
