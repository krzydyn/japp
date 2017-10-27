#ifndef __LANG_THREAD_HPP
#define __LANG_THREAD_HPP

#include <lang/String.hpp>
#include <lang/Exception.hpp>

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
	Runnable* target;
public:
	Thread() : target(null) {}
	Thread(Runnable& target) : target(&target) {}
	Thread(const String& name) : target(null){}
	Thread(Runnable& target, const String& name) : target(&target) {}
	void start() {}

	void run() {
		if (target != null) target->run();
	}
	void interrupt() {}
	boolean isInterrupted() {return isInterrupted(false);}
	boolean isInterrupted(boolean ClearInterrupted) {return false;}
	boolean isAlive() {return false;}
	void setPriority(int newPriority) {}
	int getPriority() {return 0;}
	void setName(String name) {this->name=name;}
	String getName() {return name;}
	void join(long millis) {}
	void join(long millis, int nanos) {join(millis);}
	void setDaemon(boolean on) {daemon = on;}
	boolean isDaemon() { return daemon; }
	void checkAccess() {}
	String toString() {return getClass().getName()+":"+getName();}
	Array<StackTraceElement> getStackTrace() {
		return Throwable().getStackTrace();
	}
	long getId() {return 0;}

	static Thread& currentThread();
	static void yield();
	static void sleep(long millis);
	static void sleep(long millis, int nanos);
	static boolean interrupted() {return false;}
	static void dumpStack() {
		return Throwable().printStackTrace();
	}
};

} //namespace lang

#endif
