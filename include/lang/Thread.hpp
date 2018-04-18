#ifndef __LANG_THREAD_HPP
#define __LANG_THREAD_HPP

#include <lang/Class.hpp>
#include <lang/String.hpp>
#include <lang/Exception.hpp>
#include <condition_variable>
#include <functional>
#include <mutex>


namespace std { class thread; }
namespace lang {

class InterruptedException : extends Exception {
	using Exception::Exception;
};

class Interruptible {
private:
	bool intr;
	std::mutex mtx;
	std::condition_variable cond;
public:
	Interruptible(): intr(false) {}

	void interrupt() {
		std::unique_lock<std::mutex> lock(mtx);
		intr = true;
		cond.notify_all();
	}

	void wait(long millis) {
		std::unique_lock<std::mutex> lock(mtx);
		if (intr || cond.wait_for(lock, std::chrono::milliseconds(millis)) == std::cv_status::no_timeout) {
			intr = false;
			throw InterruptedException();
		}
	}
};

interface Runnable : extends Interface {
public:
	virtual void run() = 0;
};

class ThreadGroup;
class Thread : extends Object, implements Runnable {
public:
	static const int MIN_PRIORITY = 1;
	static const int NORM_PRIORITY = 5;
	static const int MAX_PRIORITY = 10;

	interface UncaughtExceptionHandler : Interface {
	public:
		virtual void uncaughtException(const Thread& t, const Throwable& e) = 0;
	};

private:
	std::thread *thread = null;
	boolean intr = false;
	void selfupdate();

#ifdef BACKTRACE
	friend class CallTrace;
	#define BACKTRACE_SIZE 2048
	CallTrace *calltrace[BACKTRACE_SIZE];
	unsigned calltrace_size = 0;
	void tracePush(CallTrace *c);
	CallTrace *tracePop();
#endif
	Thread *parent = null;
	String  name;
	int priority;
	boolean daemon = false;
	Runnable* target = null;
	ThreadGroup *group = null;

	long stackSize;
	long tid;
	volatile int threadStatus = NEW;

	Interruptible blocker;
	boolean pendingNameChange = false;
	UncaughtExceptionHandler* uncaughtExceptionHandler = null;

	static UncaughtExceptionHandler* defaultUncaughtExceptionHandler;

	class RunnableFunction : extends Runnable {
	private:
		std::function<void()> func;
	public:
		RunnableFunction(std::function<void()> f) : func(f) {}
		void run() { func(); }
	};
	void init();

protected:
	Thread(const String& name, int status) : name(name), threadStatus(status) {}
	Thread(ThreadGroup& group,const String& name, int status) : name(name), group(&group), threadStatus(status) {}
	void setId();

public:
	Thread(const Thread& other) = delete;
	Thread(Thread&& other) = delete;
	Thread& operator=(const Thread& other) = delete;
	Thread& operator=(Thread&& o);

	Thread() {}
	Thread(Runnable& target) : target(&target) {init();}
	Thread(ThreadGroup& group, Runnable& target) : target(&target), group(&group) {init();}
	Thread(const String& name) : name(name) {init();}
	Thread(ThreadGroup& group, const String& name) : name(name), group(&group) {init();}
	Thread(Runnable& target, const String& name) : name(name), target(&target) {init();}
	Thread(ThreadGroup& group, Runnable& target, const String& name) : name(name), target(&target), group(&group) {init();}
	Thread(std::function<void()> f) : target(new RunnableFunction(f)) {init();}
	~Thread();

	virtual void start();
	void run() {TRACE;
		if (target != null) target->run();
	}
	virtual void interrupt() {intr=true;}
	virtual boolean isInterrupted() const final {return const_cast<Thread*>(this)->isInterrupted(false);}
	virtual boolean isInterrupted(boolean ClearInterrupted);
	virtual boolean isAlive() const final {return threadStatus != NEW && threadStatus != TERMINATED;}
	virtual void setPriority(int newPriority);
	virtual int getPriority() const final {return priority;}
	void setName(const String& name);
	String getName() const {return name;}
	ThreadGroup& getThreadGroup() {return *group;}
	virtual void join(long millis=0);
	virtual void join(long millis, int nanos) {TRACE;
		if (millis < 0) throw IllegalArgumentException("timeout value is negative");
		if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
		if (nanos >= 500000 || (nanos != 0 && millis == 0)) ++millis;
		join(millis);
	}
	/**
	 * when program ends, it is not waiting for end of daemon threads
	 */
	void setDaemon(boolean on) {
		checkAccess();
		if (isAlive()) throw IllegalThreadStateException();
		daemon = on;
	}
	boolean isDaemon() const { return daemon; }
	void checkAccess() const {}
	String toString() const {TRACE;return getClass().getName()+":"+getName();}
	Array<StackTraceElement> getStackTrace() const;
	long getId() const {return tid;}

	enum State {
		NEW,
		RUNNABLE,
		BLOCKED,
		WAITING,
		TIMED_WAITING,
		TERMINATED
	};

	State getState() const { return (State)threadStatus; }

	void setUncaughtExceptionHandler(UncaughtExceptionHandler* eh) {
		uncaughtExceptionHandler = eh;
	}
	const UncaughtExceptionHandler* getUncaughtExceptionHandler() {
		return uncaughtExceptionHandler != null ? uncaughtExceptionHandler : (const UncaughtExceptionHandler*)group;
	}

	static int activeCount();
	static Thread& currentThread();
	//static Map<Thread, StackTraceElement[]> getAllStackTraces() {}
	static void yield() noexcept;
	static void sleep(long millis);
	static void sleep(long millis, int nanos) {TRACE;
		if (millis < 0) throw IllegalArgumentException("timeout value is negative");
		if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
		if (nanos >= 500000 || (nanos != 0 && millis == 0)) ++millis;
		sleep(millis);
	}
	static boolean interrupted() {return false;}
	static void dumpStack() {
		Throwable("Stack trace").fillInStackTrace().printStackTrace();
	}
	static void setDefaultUncaughtExceptionHandler(UncaughtExceptionHandler* eh) {
		defaultUncaughtExceptionHandler = eh;
	}
	static UncaughtExceptionHandler* getDefaultUncaughtExceptionHandler(){return defaultUncaughtExceptionHandler;}
};

} //namespace lang

#endif
