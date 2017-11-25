#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <util/HashMap.hpp>
#include <chrono>
#include <thread>

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
	class CallLock {
		boolean& cl;
	public:
		CallLock(boolean& b) : cl(b) { cl=true; }
		~CallLock() { cl=false; }
	};
	std::thread::id mainid;
	boolean recursive=false; //guard for recursive trace call
public:
	long threadSeqNumber = 0;
	HashMap<std::thread::id,Thread*> thrmap;
	MainThread main;
	boolean ready=false;

	Threads() : thrmap(), main() {
		ready=true;
		mainid = std::this_thread::get_id();
		addThread(mainid, &main);
	}
	~Threads() {
		removeThread(mainid);
		ready=false;
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
		if (!ready) return;
		synchronized(thrmap) {
			if (recursive) return; //ignore self tracing
			CallLock cl(recursive);
			thrmap.put(id, t);
		}
	}
	void removeThread(std::thread::id id) {
		if (ready) return;
		synchronized(thrmap) {
			if (recursive) return; //ignore self tracing
			CallLock cl(recursive);
			thrmap.remove(id);
		}
	}
	Thread* getThread(std::thread::id id) {
		if (!ready) return null; //ignore self tracing
		Thread *t = null;
		synchronized(thrmap) {
			if (recursive) return null; //ignore self tracing
			CallLock cl(recursive);
			t = thrmap.get(id);
		}
		return t;
	}
} threads;
} //anoymous namespace

namespace lang {
#ifdef BACKTRACE
void Thread::tracePush(CallTrace *c) {
	if (calltrace_size < BACKTRACE_SIZE)
		calltrace[calltrace_size] = c;
	++calltrace_size;
}
CallTrace *Thread::tracePop() {
	if (calltrace_size == 0) return null;
	--calltrace_size;
	if (calltrace_size < BACKTRACE_SIZE)
		return calltrace[calltrace_size];
	return null;
}

void CallTrace::add() {
	std::thread::id id = std::this_thread::get_id();
	t = threads.getThread(id);
	if (t!=null) t->tracePush(this);
}
CallTrace::~CallTrace() {
	if (t!=null) {
		CallTrace *c = t->tracePop();
		if (c == null) return ;
		if (c != this) std::cerr << "stack mismach" << std::endl;
	}
}
#endif

Array<StackTraceElement> Thread::getStackTrace() const {
#ifdef BACKTRACE
	Array<StackTraceElement> st(calltrace_size);
	for (int i=0; i < st.length; ++i) {
		CallTrace *ct = calltrace[st.length - i - 1];
		st[i] = StackTraceElement(ct->func,ct->file,ct->line);
	}
	return std::move(st);
#else
	return std::move(Throwable().getStackTrace());
#endif
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
