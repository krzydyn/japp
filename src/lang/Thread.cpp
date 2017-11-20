#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <util/HashMap.hpp>
#include <chrono>
#include <thread>

namespace lang {

namespace {
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
public:
	MainThread() : Thread("main", RUNNABLE) {}
};

class Threads : extends Object {
	std::thread::id mainid;
public:
	long threadSeqNumber = 0;
	HashMap<std::thread::id,Thread*> thrmap;
	MainThread main;
	Threads() : thrmap(), main() {
		mainid = std::this_thread::get_id();
		std::cerr << "adding main" << std::endl;
		addThread(mainid, &main);
		std::cerr << "adding main done" << std::endl;
	}
	~Threads() {
		removeThread(mainid);
	}
	long nextThreadNumber() {
		long tid;
		synchronized(*this) {
			tid = threadSeqNumber;
			++threadSeqNumber;
		}
		return tid;
	}
	void addThread(std::thread::id id, Thread* t) {
		synchronized(thrmap) { thrmap.put(id, t); }
	}
	void removeThread(std::thread::id id) {
		synchronized(thrmap) { thrmap.remove(id); }
	}
	Thread* getThread(std::thread::id id) {
		Thread *t = null;
		synchronized(thrmap) { t = thrmap.get(id); }
		return t;
	}
} threads;

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

	tid = threads.nextThreadNumber();
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
		threads.addThread(thrid, this);
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
		threads.removeThread(thrid);
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
	std::thread::id id = std::this_thread::get_id();
	Thread *t = threads.getThread(id);
	if (t == null) {
		System.err.println("FATAL: thread not found: " + String::valueOf(id));
		return threads.main;
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
