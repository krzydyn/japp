#include <lang/Object.hpp>
#include <lang/Class.hpp>
#include <lang/Number.hpp>
#include <lang/System.hpp>
#include <lang/Thread.hpp>

#include <exception>
#include <stdexcept> //std::exception_ptr

#include <unistd.h> // write
#include <signal.h> // signal, SIGxxx
#include <execinfo.h> //backtrace
#include <dlfcn.h> //dladdr
#ifdef __GNUG__ // gnu C++ compiler
#include <memory>
#include <cxxabi.h>
#endif

// addr2line for *.so
//https://stackoverflow.com/questions/7556045/how-to-map-function-address-to-function-in-so-files
// addr2line -j .text -e libtst.so 0x26887
// where 0x26887 is 0xb77dc887(fun2 addr+offset)-0xb77b6000 (lib starting addr)
//
// http://blog.bigpixel.ro/2010/09/stack-unwinding-stack-trace-with-gcc/
// readelf --debug-dump=decodedline  a.out

extern "C" {
void __cxa_pure_virtual() {
	throw Error("Pure virtual method called");
}
typedef void (*cxa_throw_type) (void *, struct type_info *, void (*) (void *));
void __cxa_throw(void* thrown_exception, void* _tinfo, void (*dest)(void*)) {
	static cxa_throw_type old_handler=null;
	struct type_info* tinfo = (struct type_info*) _tinfo;
	if (!old_handler) {
		old_handler = (cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw");
	}
	const Object *o = (const Object *)thrown_exception;
	if (instanceof<Throwable>(o)) {
		Throwable* ex = (Throwable*)o;
		//std::cerr << __FILE__ << "(" << __LINE__<< "): thrown " << ex->getClass().getName().cstr() << std::endl;
		if (ex->getStackTrace().length == 0)
			ex->fillInStackTrace();
	}
	else if (instanceof<std::exception>(o)) {
		std::exception *ex = (std::exception*)o;
		std::cerr << __FILE__ << "(" << __LINE__<< "): thrown std::exception: " << ex->what() << std::endl;
	}
	else {
		//std::cerr << __FILE__ << "(" << __LINE__<< "): thrown unknown exception: " << Object::getClass(typeid(*o)).getName().cstr() << std::endl;
		LOGE("unknown exception: %s", Object::getClass(typeid(*o)).getName().cstr());
	}
	old_handler(thrown_exception, tinfo, dest);
	std::_Exit(EXIT_FAILURE);
}
}

namespace {
ArrayList<Class*>& classmap() {
	static ArrayList<Class*> *a = null;
	if (!a) a = new ArrayList<Class*>(5);
	return *a;
}

boolean initialize();
static const boolean SET_TERMINATE = initialize();

[[noreturn]] void terminate_hook();
void signal_handle(int signum);
boolean initialize() {
	(void)SET_TERMINATE;
	std::set_terminate(terminate_hook);
	signal(SIGFPE, signal_handle);
	signal(SIGABRT, signal_handle);
	//signal(SIGSEGV, signal_handle);
	signal(SIGINT, signal_handle);
	return true;
}

std::string demangle(const std::string& name) {
	if (name.empty()) return "??";
#ifdef __GNUG__ // gnu C++ compiler
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr<char, decltype(&std::free)> ptr(__cxxabiv1::__cxa_demangle(name.c_str(), null, &len, &status), &std::free);
	return status == 0 ? ptr.get() : name;
#else
	return name;
#endif
}
String getSimpleBinaryName() { return ""; }
//TODO https://github.com/CyberGrandChallenge/binutils/blob/master/binutils/addr2line.c
//     osx(atos): sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr);
//     linux:     sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr);
Array<StackTraceElement>& captureStackTrace(Array<StackTraceElement>& stackTrace, const int skip) {
	const int depth = 50;
	void *trace[depth];
	int got = ::backtrace(trace, depth);
	if (got <= skip) {
		return stackTrace;
	}

	got -= skip;
	stackTrace = Array<StackTraceElement>(got);
	for (int i = 0; i < got; ++i) {
		Dl_info info;
		//std::cerr << "addr 0x" << std::hex << (long)trace[i+skip] << std::endl;
		std::string addr = "[0x"+Long::toHexString((long)trace[i+skip]).intern()+"]";
		if (dladdr(trace[i+skip], &info) != 0) {
			//dli_fname - path of shared object (exe or so)
			//dli_fbase - Base adress of shared object
			//dli_sname - Name of nearest symbol
			//dli_saddr - Exact address of symbol
			std::string path = info.dli_fname;
			if (path.rfind('/') != std::string::npos) path = path.substr(path.rfind('/')+1);
			if (info.dli_sname == null) info.dli_sname="";
			std::string func = demangle(info.dli_sname);
			std::string offs;
			if (info.dli_saddr != 0)
				offs = "+" + std::to_string((long)trace[i+skip] - (long)info.dli_saddr);
			else
				offs = "";
			stackTrace[i] = StackTraceElement(func+offs+" "+path+addr, "", 0);
		}
		else {
			stackTrace[i] = StackTraceElement(addr, "", 0);
		}
	}
	return stackTrace;
}
#ifdef BACKTRACE
void captureStack2(Array<StackTraceElement>& stackTrace) {
	Thread& t = Thread::currentThread();
	stackTrace = t.getStackTrace();
}
#endif
//FIXME do not call any c lib function from a handler (only async-safe can be used)
//      also don't throw exception
//  read SIGNAL-SAFETY(7) for list of async-safe-functions
void signal_handle(int signum) {
	//System::err.printf("%sReceived signal %d %s\n", "\x1b[1;37m", signum, "\x1b[m");
	static char buf[200];
	int n = sprintf(buf, "%sReceived signal %d %s\n", "\x1b[1;37m", signum, "\x1b[m");
	//System::err ... causes dead lock
	::write(STDERR_FILENO , buf, n);

	Array<StackTraceElement> st;
	captureStackTrace(st, 3);
	if (signum == SIGINT) {
		InterruptedException ex("SIGINT");
		ex.setStackTrace(st);
		throw ex;
	}
	if (signum == SIGFPE) {
		ArithmeticException ex("SIGFPE");
		ex.setStackTrace(st);
		throw ex;
	}

	Throwable t("Signal " + String::valueOf(signum));
	t.setStackTrace(st);
	t.printStackTrace();
	terminate_hook();
}

[[noreturn]]
void terminate_hook() {
	std::set_terminate(null); // avoid loop
	std::exception_ptr ep = std::current_exception();
	if (ep != null) {
		System::err.println("Uncatched exception received");
		try {
			std::rethrow_exception(ep);
		} catch (const Throwable& e) {
			e.printStackTrace();
		} catch (const std::exception& e) {
			Array<StackTraceElement> st;
			Throwable t(Object::getClass(typeid(e)).getName() + ":" + e.what());
			t.setStackTrace(captureStackTrace(st,3));
			t.printStackTrace();
		} catch (...) {
			Array<StackTraceElement> st;
			Throwable t("* unknown exception *");
			t.setStackTrace(captureStackTrace(st,3));
			t.printStackTrace();
		}
	}
	else {
		std::cerr << "Terminated, no exception" << std::endl;
	}
	std::_Exit(EXIT_FAILURE);
	//std::abort();
}

class ArrayClass : extends Class {
public:
	ArrayClass(const std::type_info& t) : Class(t) {}
	boolean isArray() const {return true;}
};

}

namespace lang {

long Object::null_val = 0;

Throwable::Throwable(const String& msg, Throwable *c) : detailMessage(msg), cause(c) {
	threadInfo = Thread::currentThread().getName();
}
Throwable& Throwable::initCause(const Throwable *cause) {
	if (this->cause != this)
		throw IllegalStateException("Can't overwrite cause with " + String::valueOf(cause), this);
	if (cause == this)
		throw IllegalArgumentException("Self-causation not permitted", this);
	this->cause = cause;
	return *this;
}
String Throwable::toString() const {
	String s = getClass().getName();
	String message = getLocalizedMessage();
	return (message != null) ? (s + ": " + message) : (s + ": null msg");
}
Throwable& Throwable::fillInStackTrace() {
	(void)captureStackTrace;
#ifdef BACKTRACE
	captureStack2(stackTrace);
#else
	captureStackTrace(stackTrace, 3);
#endif
	return *this;
}
void Throwable::printStackTrace() const {TRACE;
	io::PrintStream& s = System::err;
	s.print("\x1b[0;31m");
	printStackTrace(s);
	s.print("\x1b[m");
}
void Throwable::printStackTrace(io::PrintStream& s) const {TRACE;
	synchronized(s) {
	s.println("[T]Exception in thread \"" + threadInfo + "\" " + this->toString());
	for (int i=0; i < stackTrace.length; ++i) {
		s.print("\tat ");
		s.println(stackTrace[i].toString());
	}
	const Throwable *ourCause = getCause();
	if (ourCause != null) {
		s.print("Caused by ");
		cause->printStackTrace(s);
	}
	s.flush();
	}
}

String Class::toString() const {
	String c = isInterface() ? "interface " : (isPrimitive() ? "" : "class ");
	return c + getName();
}
String Class::toGenericString() const {return toString();}
String Class::getSimpleName() const {TRACE;
	String simpleName = getSimpleBinaryName();
	if (simpleName.isEmpty()) { // top level class
		simpleName = getName();
		int i = simpleName.lastIndexOf(":");
		if (i >= 0) return simpleName.substring(i+1); // strip the package name
	}
	return simpleName;
}
String Class::getName() const {TRACE;return demangle(type.name());}
String Class::getCanonicalName() const {TRACE;return getName();}


Class *Object::findClass(const std::type_info& type) {
	ArrayList<Class*>& cm = classmap();
	synchronized(cm) {
	for (SharedIterator<Class*> i = cm.iterator(); i->hasNext(); ) {
		Class *c = i->next();
		if (c->type == type) return c;
	}
	}
	return null;
}
void Object::registerClass(Class *c) {
	ArrayList<Class*>& cm = classmap();
	synchronized(cm) {
		Class *o = Object::findClass(c->type);
		if (o == c) return ;
		if (o) cm.remove(o);
		cm.add(c);
	}
}
const Class& Object::getClass() const {TRACE;
	return getClass(typeid(*this));
}
const Class& Object::getClass(const std::type_info& type) {
	Class *c = null;
	ArrayList<Class*>& cm = classmap();
	synchronized(cm) {
		c = Object::findClass(type);
		if (!c) Object::registerClass(c=new Class(type));
	}
	return *c;
}

class CondMonitor {
private:
	const Object *obj;
	std::condition_variable cond;
public:
	CondMonitor(const Object *o) : obj(o) {}
	void wait(long t);
	void notify();
	void notifyAll();
};

void CondMonitor::wait(long t) {
	//cond.wait_for(lock, std::chrono::milliseconds(millis));
}
void CondMonitor::notify() {
	cond.notify_one();
}
void CondMonitor::notifyAll() {
	cond.notify_all();
}

Object::~Object() { delete mtx; delete cond; }
Object& Object::clone() const {TRACE;
	throw CloneNotSupportedException();
}
String Object::toString() { return ((const Object*)this)->toString(); }
String Object::toString() const {
	return getClass().getName() + "@" + Integer::toHexString(hashCode());
}

void Object::notify() {
	if (cond) cond->notify();
}
void Object::notifyAll() {
	if (cond) cond->notifyAll();
}
void Object::wait(long timeout) {
	if (timeout < 0) throw IllegalArgumentException("timeout value is negative");
	if (cond == null) cond = new CondMonitor(this);
	cond->wait(timeout);
}

void Object::wait(long timeout, int nanos) {
	if (timeout < 0) throw IllegalArgumentException("timeout value is negative");
	if (nanos < 0 || nanos > 999999) throw IllegalArgumentException("nanosecond timeout value out of range");
	if (nanos > 0) timeout++;
	wait(timeout);
}

void AbstractArray::registerArrayClass(const std::type_info& type) {
	ArrayList<Class*>& cm = classmap();
	synchronized(cm) {
		Class *c = Object::findClass(type);
		if (!c) Object::registerClass(new ArrayClass(type));
	}
}
void AbstractArray::checkArrayBounds(int i, int l) {
	if (i < 0 || i >= l) throw IndexOutOfBoundsException(i);
}

} //namespace lang
