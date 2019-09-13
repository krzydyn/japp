#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <lang/ThreadGroup.hpp>
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
		pthread_setname_np(name.cstr());
		pending = false;
	}
	else pending = true;
	#else
	pending = false;
	pthread_setname_np(thread.native_handle(), name.cstr());
	#endif
}
class MainThreadGroup : extends ThreadGroup {
public:
	MainThreadGroup() : ThreadGroup("main",null) {}
};
class MainThread : extends Thread {
public:
	MainThread(ThreadGroup& g) : Thread(g, "main", RUNNABLE) {}
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
	Thread unknownThread;
	long threadSeqNumber = 0;
	long threadInitNumber = 0;
	HashMap<std::thread::id,Thread*> thrmap;
	MainThreadGroup maingroup;
	MainThread main;

	Threads() : thrmap(), main(maingroup) {
		mainid = std::this_thread::get_id();
		addThread(mainid, &main);
		std::cout << "Threads init done" << std::endl;
		unknownThread.setName("THREAD?");
	}

	~Threads() {
		std::cout << "Threads destructor" << std::endl;
		removeThread(mainid);
	}
	long nextThreadNum() {
		long n = 0;
		synchronized(*this) {
			n = threadInitNumber++;
		}
		return n;
	}
	long nextThreadID() {
		long tid = 0;
		synchronized(*this) {
			tid = ++threadSeqNumber;
		}
		return tid;
	}
	void addThread(std::thread::id id, Thread* t) {
		synchronized(thrmap) {
			if (recursive) return; //ignore self tracing
			CallLock cl(recursive);
			thrmap.put(id, t);
		}
	}
	void removeThread(std::thread::id id) {
		synchronized(thrmap) {
			if (recursive) return; //ignore self tracing
			CallLock cl(recursive);
			thrmap.remove(id);
		}
	}
	Thread* getThread(std::thread::id id) {
		Thread *t = null;
		synchronized(thrmap) {
			if (recursive) return null; //ignore self tracing
			CallLock cl(recursive);
			if (!thrmap.containsKey(id)) t = &unknownThread;
			else t = thrmap.get(id);
		}
		return t;
	}
};

//threads lazy initialization
Threads& threads() {
	static Threads t;
	return t;
}
} //anonymous namespace

namespace lang {
Thread::UncaughtExceptionHandler* Thread::defaultUncaughtExceptionHandler = null;

void Thread::init() {
	parent = &Thread::currentThread();
	if (group == null) {
		group = &parent->getThreadGroup();
	}
	if (group != null)
		group->addUnstarted();
	daemon = parent->isDaemon();
	priority = parent->getPriority();
	tid = threads().nextThreadID();
}

boolean Thread::isInterrupted(boolean ClearInterrupted) {
	boolean r = intr;
	if (ClearInterrupted) intr=false;
	return r;
}

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
	t = threads().getThread(id);
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
	if (threadStatus != NEW) throw IllegalThreadStateException();

	priority = o.priority;
	daemon = o.daemon;
	tid = o.tid;

	threadStatus = o.threadStatus; o.threadStatus = TERMINATED;
	target = o.target; o.target = null;
	name = std::move(o.name);
	thread = o.thread; o.thread = null;
	return *this;
}
// Executing the destructor for a joinable thread results in calling std::terminate.
Thread::~Thread() {
	if (thread == null) return ;
	try {
		join();
		delete thread;
		thread = null;
		if (instanceof<RunnableFunction>(target)) {
			LOGD("delete RunnableFunction");
			delete target;
		}
	}catch(const Throwable& e) {
		LOGE("Exeption in ~Thread() %s", getName().cstr());
		e.printStackTrace();
	}catch(...) {
		LOGE("Unknown Exeption in ~Thread() %s", getName().cstr());
	}
	System::out.println(getName() + " destructor");
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
	if (threadStatus != NEW) throw IllegalThreadStateException();

	if (group) group->add(this);

	boolean started = false;
	try {
		start0();
		started = true;
	} catch (...) {
		if (!started) {
			group->threadStartFailed(this);
		}
		throw;
	}
}

/*
 * []	Capture nothing (or, a scorched earth strategy?)
 * [&]	Capture any referenced variable by reference
 * [=]	Capture any referenced variable by making a copy
 * [=, &foo]	Capture any referenced variable by making a copy, but capture variable foo by reference
 * [bar]	Capture bar by making a copy; don't copy anything else
 * [this]	Capture the this pointer of the enclosing class
 */
void Thread::start0() {
	if (name.length() == 0) {
		name += "Thread-" + String::valueOf(tid);
	}
	pendingNameChange = true;
	threadStatus = RUNNABLE;
	LOGN("Starting new thread %s", getName().cstr());
	this->thread = new std::thread([&] {
		std::thread::id thrid = std::this_thread::get_id();
		threads().addThread(thrid, this);
		Thread::selfupdate();
		try {
			//do { Thread::yield(); } while (threadStatus == NEW);
			if (threadStatus == RUNNABLE) {
				LOGN("Thread started %s", getName().cstr());
				setPriority(priority);
				run();
			}
		} catch(const Throwable& e) {
			e.printStackTrace();
		} catch (const std::exception& e) {
			Throwable t(Object::getClass(typeid(e)).getName() + ":" + e.what());
			t.fillInStackTrace().printStackTrace();
		} catch (...) {
			Throwable().fillInStackTrace().printStackTrace();
		}
		threadStatus = TERMINATED;
		LOGN("Thread finished %s", getName().cstr());
		threads().removeThread(thrid);
	});
	//Thread::yield();
	if (daemon) thread->detach();
}
void Thread::join(long millis) {TRACE;
	if (thread == null) throw NullPointerException();
	synchronized(*this) {
	if (thread->joinable()) {
		threadStatus = WAITING;
		thread->join();
		threadStatus = RUNNABLE;
	}
	}
}
void Thread::selfupdate() {
	if (!thread) return ;
	if (pendingNameChange) setNativeName(*thread, name, pendingNameChange);
}

// static methods
Thread& Thread::currentThread() {
	std::thread::id id = std::this_thread::get_id();
	Thread *t = threads().getThread(id);
	if (t == null) {
		System::err.println("FATAL: thread not found: " + String::valueOf(id));
		std::abort();
	}
	return *t;
}

void Thread::yield() noexcept {
	Thread::currentThread().selfupdate();
	std::this_thread::yield();
}
void Thread::sleep(long millis) {
	Thread& t = currentThread();
	synchronized(t) {
	t.threadStatus = WAITING;
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
	t.threadStatus = RUNNABLE;
	}
}

int Thread::activeCount() {
	return currentThread().getThreadGroup().activeCount();
}

} //namespace lang
