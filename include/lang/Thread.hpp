#ifndef __LANG_THREAD_HPP
#define __LANG_THREAD_HPP

#include <lang/Class.hpp>
#include <lang/String.hpp>
#include <lang/Exception.hpp>
#include <thread>

namespace lang {

interface Runnable : extends Interface {
public:
	virtual void run() = 0;
};

class Thread : extends Object, implements Runnable {
private:
	static long threadSeqNumber;

	String  name;
	int     priority;
	boolean daemon = false;
	Runnable* target;
	long stackSize;
	long tid;
	volatile int threadStatus = NEW;

	std::thread thread;
public:
	Thread() : target(null) {}
	Thread(Runnable& target) : target(&target) {}
	Thread(const String& name) : name(name), target(null){}
	Thread(Runnable& target, const String& name) : name(name), target(&target) {}
	//TODO suport lamba as target
	~Thread() { if (threadStatus == RUNNABLE) join(); }

	void start();
	void run() {
		if (target != null) target->run();
	}
	void interrupt() {}
	boolean isInterrupted() {return isInterrupted(false);}
	boolean isInterrupted(boolean ClearInterrupted) {return false;}
	boolean isAlive() {return false;}
	void setPriority(int newPriority) {}
	int getPriority() {return 0;}
	void setName(const String& name);
	String getName() {return name;}
	void join(long millis=0) {thread.join();}
	void join(long millis, int nanos) {
		if (millis < 0) throw IllegalArgumentException("timeout value is negative");
		if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
		if (nanos >= 500000 || (nanos != 0 && millis == 0)) ++millis;
		join(millis);
	}
	void setDaemon(boolean on) {
		checkAccess();
		if (isAlive()) throw IllegalThreadStateException();
		daemon = on;
	}
	boolean isDaemon() { return daemon; }
	void checkAccess() {}
	String toString() {return getClass().getName()+":"+getName();}
	Array<StackTraceElement> getStackTrace() {
		return Throwable().getStackTrace();
	}
	long getId() {return tid;}

	enum State {
		NEW,
		RUNNABLE,
		BLOCKED,
		WAITING,
		TIMED_WAITING,
		TERMINATED
	};

	State getState() { return (State)threadStatus; }

	static int activeCount() {return 0;}
	static Thread& currentThread();
	//static Map<Thread, StackTraceElement[]> getAllStackTraces() {}
	static void yield();
	static void sleep(long millis);
	static void sleep(long millis, int nanos) {
		if (millis < 0) throw IllegalArgumentException("timeout value is negative");
		if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
		if (nanos >= 500000 || (nanos != 0 && millis == 0)) ++millis;
		sleep(millis);
	}
	static boolean interrupted() {return false;}
	static void dumpStack() {
		Throwable("Stack trace").printStackTrace();
	}
private:
	void runHelper();
	void setNativeName(const String& name);
};

} //namespace lang

#endif
