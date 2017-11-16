#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <util/HashMap.hpp>
#include <chrono>
#include <thread>

namespace lang {

namespace {
Object threadSeqSync;
HashMap<std::thread::id,Thread*> thrmap;
long threadSeqNumber = 0;

class MainThread : extends Thread {
private:
	std::thread::id thrid;
public:
	MainThread() : Thread("main") {
		thrid = std::this_thread::get_id();
		thrmap.put(thrid, this);
	}
	~MainThread() {
		thrmap.remove(thrid);
	}
} main_thread;

void setNativePriority(std::thread& thread, int priority) {
	int policy;
	sched_param sch_params;
	pthread_getschedparam(thread.native_handle(), &policy, &sch_params);

	sch_params.sched_priority = priority;
	pthread_setschedparam(thread.native_handle(), policy, &sch_params);
}
void setNativeName(std::thread& thread, const String& name) {
#ifdef __APPLE__
	pthread_setname_np(name.intern().c_str());
#else
	pthread_setname_np(thread.native_handle(), name.intern().c_str());
#endif
}
}

void Thread::setId() {
	synchronized(threadSeqSync) {
		tid = threadSeqNumber;
		++threadSeqNumber;
	}
}
Thread& Thread::operator=(Thread&& o) {
	if (threadStatus != NEW) {
		throw IllegalThreadStateException();
	}
	priority = o.priority;
	daemon = o.daemon;
	tid = o.tid;
	threadStatus = o.threadStatus; o.threadStatus = TERMINATED;
	target = o.target; o.target = null;
	name = std::move(o.name);
	thread = o.thread; o.thread = null;
	return *this;
}
Thread::~Thread() {
	if (thread == null) return ;
	System.out.println(name + " destructor");
	if (thread != null) {
		thread->join();
		delete thread;
	}
}

void Thread::yield() noexcept { std::this_thread::yield(); }
void Thread::join(long millis) {TRACE;
	if (isAlive()) thread->join();
}
void Thread::sleep(long millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Thread::setPriority(int newPriority) {
	setNativePriority(*thread, newPriority);
}
void Thread::setName(const String& name) {
	this->name=name;
	if (threadStatus != NEW) {
		setNativeName(*thread, name);
	}
}

Thread& Thread::currentThread() {
	std::thread::id thrid = std::this_thread::get_id();
	Thread *t = null;
	synchronized(thrmap) { t = thrmap.get(thrid); }
	if (t == null) {
		System.err.println("FATAL: thread not found: " + String::valueOf(thrid));
		return main_thread;
	}
	return *t;
}

void Thread::start() {
	if (threadStatus != NEW) {
		throw IllegalThreadStateException();
	}

	setId();
	if (name.length() == 0) name = "Thread-" + String::valueOf(tid);

	this->thread = new std::thread([=] {
		std::thread::id thrid = std::this_thread::get_id();
		synchronized(thrmap) { thrmap.put(thrid, this); }
		try {
			setNativeName(*thread, name);
			do { yield(); } while (threadStatus == NEW);
			if (threadStatus == RUNNABLE) run();
		} catch(const Throwable& e) {
			e.printStackTrace(System.err);
		} catch (const std::exception& e) {
			Throwable(e.what()).fillInStackTrace().printStackTrace();
		} catch (...) {
			Throwable().fillInStackTrace().printStackTrace();
		}
		System.out.println(name + " terminated");
		threadStatus = TERMINATED;
		synchronized(thrmap) { thrmap.remove(thrid); }
	});
	threadStatus = RUNNABLE;
}

} //namespace lang
