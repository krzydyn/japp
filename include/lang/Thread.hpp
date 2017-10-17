#ifndef __LANG_THREAD_HPP
#define __LANG_THREAD_HPP

#include <lang/String.hpp>

namespace lang {

class Runnable {
public:
	virtual void run() = 0;
};

class Thread : public Object,public Runnable {
private:
	String  name;
	int     priority;
	boolean daemon = false;
	Runnable& target;
public:
	Thread() : target(*((Runnable*)null)) {}
	Thread(Runnable& target) : target(target) {}
	Thread(const String& name) : target(*((Runnable*)null)){}
	Thread(Runnable& target, const String& name) : target(target) {}
	void start() {}

	void run() {
		if (target != null) target.run();
	}
	void interrupt() {}
	boolean isInterrupted() {return isInterrupted(false);}
	boolean isInterrupted(boolean ClearInterrupted) {}
	boolean isAlive() {}
	void setPriority(int newPriority) {}
	int getPriority() {}
	void setName(String name) {}
	String getName() {}
	void join(long millis) {}
	void join(long millis, int nanos) {join(millis);}
	void setDaemon(boolean on) {daemon = on;}
	boolean isDaemon() { return daemon; }
	void checkAccess() {}
	String toString() {}
	//StackTraceElement[] getStackTrace() { }
	long getId() {}

	static Thread& currentThread();
	static void yield();
	static void sleep(long millis);
	static void sleep(long millis, int nanos);
	static boolean interrupted() {}
	static void dumpStack() {}
};

} //namespace lang

#endif
