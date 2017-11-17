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

void setNativePriority(std::thread& thread, int priority) {
	int policy;
	sched_param sch_params;
	pthread_getschedparam(thread.native_handle(), &policy, &sch_params);

	sch_params.sched_priority = priority;
	pthread_setschedparam(thread.native_handle(), policy, &sch_params);
}
void setNativeName(std::thread& thread, const String& name, boolean& pending) {
	#ifdef __APPLE__
	if (thread.get_id() == std::this_thread::get_id()) {
		pthread_setname_np(name.intern().c_str());
		pending = false;
	}
	else pending = true;
	#else
	pending = false;
	pthread_setname_np(thread.native_handle(), name.intern().c_str());
	#endif
}

class MainThread : extends Thread {
private:
	std::thread::id thrid;
public:
	MainThread() : Thread("main", RUNNABLE) {
		thrid = std::this_thread::get_id();
		thrmap.put(thrid, this);
	}
	~MainThread() {
		thrmap.remove(thrid);
	}
} main_thread;

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
	System.out.println(getName() + " destructor");
	if (thread != null) {
		thread->join();
		delete thread;
	}
}
void Thread::setId() {
	synchronized(threadSeqSync) {
		tid = threadSeqNumber;
		++threadSeqNumber;
	}
}
void Thread::setPriority(int newPriority) {
	setNativePriority(*thread, newPriority);
}
void Thread::setName(const String& name) {
	if (this->name.equals(name)) return ;
	this->name=name;
	if (threadStatus != NEW) {
		setNativeName(*thread, name, pendingNameChange);
	}
}
void Thread::start() {
	if (threadStatus != NEW) {
		throw IllegalThreadStateException();
	}

	setId();
	parent = &Thread::currentThread();
	if (name.length() == 0) {
		name += parent->getName() + "::";
		name += "Thread-" + String::valueOf(tid);
	}
	else {
		name = parent->getName() + "::" + name;
	}
	pendingNameChange = true;
	this->thread = new std::thread([=] {
		std::thread::id thrid = std::this_thread::get_id();
		synchronized(thrmap) { thrmap.put(thrid, this); }
		try {
			do { Thread::yield(); } while (threadStatus == NEW);
			if (threadStatus == RUNNABLE) run();
		} catch(const Throwable& e) {
			e.printStackTrace(System.err);
		} catch (const std::exception& e) {
			Throwable(e.what()).fillInStackTrace().printStackTrace();
		} catch (...) {
			Throwable().fillInStackTrace().printStackTrace();
		}
		System.out.println(getName() + " terminated");
		threadStatus = TERMINATED;
		synchronized(thrmap) { thrmap.remove(thrid); }
	});
	threadStatus = RUNNABLE;
}
void Thread::join(long millis) {TRACE;
	if (isAlive()) thread->join();
}
void Thread::selfupdate() {
	if (!thread) return ;
	if (pendingNameChange) setNativeName(*thread, name, pendingNameChange);
}

// static functions
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

void Thread::yield() noexcept {
	Thread::currentThread().selfupdate();
	std::this_thread::yield();
}
void Thread::sleep(long millis) {
	Thread::currentThread().selfupdate();
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

} //namespace lang
