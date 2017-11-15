#ifndef __LANG_THREAD_HPP
#define __LANG_THREAD_HPP

#include <lang/Class.hpp>
#include <lang/String.hpp>
#include <lang/Exception.hpp>
#include <mutex>
#include <condition_variable>

namespace std { class thread; }
namespace lang {

class Interruptible
{
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
            //throw InterruptedException;
        }
    }
private:
    bool intr;
    std::mutex mtx;
    std::condition_variable cond;
};

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
	Interruptible interruptor;

	std::thread *thread = null;
protected:
	void setId();

public:
	Thread(const Thread& other) = delete;
	Thread(Thread&& other) = delete;
	Thread& operator=(const Thread& other) = delete;
	Thread& operator=(Thread&& o);

	//TODO suport lamba as target in constructor
	Thread() : target(null) {}
	Thread(Runnable& target) : target(&target) {}
	Thread(const String& name) : name(name), target(null){}
	Thread(Runnable& target, const String& name) : name(name), target(&target) {}
	~Thread();

	void start();
	void run() {TRACE;
		if (target != null) target->run();
	}
	void interrupt() {}
	boolean isInterrupted() const {return isInterrupted(false);}
	boolean isInterrupted(boolean ClearInterrupted) const {return false;}
	boolean isAlive() const {return threadStatus != NEW && threadStatus != TERMINATED;}
	void setPriority(int newPriority);
	int getPriority() const {return priority;}
	void setName(const String& name);
	String getName() const {return name;}
	void join(long millis=0);
	void join(long millis, int nanos) {TRACE;
		if (millis < 0) throw IllegalArgumentException("timeout value is negative");
		if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
		if (nanos >= 500000 || (nanos != 0 && millis == 0)) ++millis;
		join(millis);
	}
	void setDaemon(boolean on) {TRACE;
		checkAccess();
		if (isAlive()) throw IllegalThreadStateException();
		daemon = on;
	}
	boolean isDaemon() const { return daemon; }
	void checkAccess() const {}
	String toString() const {TRACE;return getClass().getName()+":"+getName();}
	Array<StackTraceElement> getStackTrace() {TRACE;
		return std::move(Throwable().getStackTrace());
	}
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

	static int activeCount() {return 0;}
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
	static void dumpStack() {TRACE;
		Throwable("Stack trace").printStackTrace();
	}
};

} //namespace lang

#endif
