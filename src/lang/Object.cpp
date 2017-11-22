#include <lang/Object.hpp>
#include <lang/Class.hpp>
#include <lang/Number.hpp>
#include <lang/System.hpp>
#include <lang/Thread.hpp>

#include <exception>
#include <stdexcept> //std::exception_ptr

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

typedef void (*cxa_throw_type) (void *, struct type_info *, void (*) (void *));
extern "C"
void __cxa_throw(void* thrown_exception, void* _tinfo, void (*dest)(void*)) {
	static cxa_throw_type old_handler=null;
	struct type_info* tinfo = (struct type_info*) _tinfo;
	if (!old_handler) {
		old_handler = (cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw");
	}
	//std::printf("throw...%p\n", thrown_exception);
	const Object *o = (const Object *)thrown_exception;
	if (instanceOf<Throwable>(o)) {
		((Throwable*)o)->fillInStackTrace();
	}
	old_handler(thrown_exception, tinfo, dest);
	_Exit(-1);
}

namespace {
boolean initialize();
static const boolean SET_TERMINATE = initialize();

void terminate_hook();
void signal_handle(int signum);
boolean initialize() {
	(void)SET_TERMINATE;
	std::set_terminate(terminate_hook);
	signal(SIGFPE, signal_handle);
	signal(SIGSEGV, signal_handle);
	return true;
}

#ifdef BACKTRACE
#define BACKTRACE_SIZE 2048
CallTrace *calltrace[BACKTRACE_SIZE];
unsigned calltrace_size = 0;
void tracePush(CallTrace *c) {
	if (calltrace_size < BACKTRACE_SIZE)
		calltrace[calltrace_size] = c;
	++calltrace_size;
}
CallTrace *tracePop() {
	if (calltrace_size == 0) return null;
	--calltrace_size;
	if (calltrace_size < BACKTRACE_SIZE)
		return calltrace[calltrace_size];
	return null;
}
unsigned traceSize() { return calltrace_size; }
#endif

std::string demangle(const std::string& name) {
	if (name.empty()) return "??";
#ifdef __GNUG__ // gnu C++ compiler
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr< char, decltype(&std::free) > ptr(
		__cxxabiv1::__cxa_demangle(name.c_str(), null, &len, &status), &std::free);
	return status == 0 ? ptr.get() : name;
#else
	return name;
#endif
}
String getSimpleBinaryName() { return ""; }
//TODO https://github.com/CyberGrandChallenge/binutils/blob/master/binutils/addr2line.c
//     osx(atos): sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr);
//     linux:     sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr);
Array<StackTraceElement>& captureStackTrace(Array<StackTraceElement>& stackTrace, const int skip=3) {
	const int depth = 50;
	void *trace[depth];
	int got = ::backtrace(trace, depth);
	printf("::backtrace = %d\n",got);
	if (got <= skip) {
		//System.err.println("no backtrace");
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
			std::string offs = "+" + std::to_string((long)trace[i+skip] - (long)info.dli_saddr);
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
	stackTrace = Array<StackTraceElement>(traceSize());
	for (int i = 0; i < stackTrace.length; ++i) {
		CallTrace *ct = calltrace[stackTrace.length - i - 1];
		if (ct == null) {
			stackTrace[i] = StackTraceElement("","",0);
		}
		else {
			//std::printf("bt[%d]: f='%s'  @ '%s:%d'\n", i, ct->func,ct->file,ct->line);
			stackTrace[i] = StackTraceElement(ct->func,ct->file,ct->line);
		}
	}
}
#endif
void signal_handle(int signum) {
	System.err.println("Received signal " + String::valueOf(signum));
	if (signum == SIGFPE) throw ArithmeticException("SIGFPE");
	Array<StackTraceElement> st;
	captureStackTrace(st);
	for (int i=0; i < st.length; ++i) {
		System.err.println(st[i].toString());
	}
	//Throwable().fillInStackTrace().printStackTrace();
	terminate_hook();
}
[[noreturn]]
void terminate_hook() {
	std::set_terminate(null); // avoid loop
	std::exception_ptr ep = std::current_exception();
	if (ep != null) {
		System.err.println("Uncatched exception received");
		try {
			std::rethrow_exception(ep);
		} catch (const Throwable& e) {
			e.printStackTrace();
		} catch (const std::exception& e) {
			Array<StackTraceElement> st;
			Throwable t(e.what());
			t.setStackTrace(captureStackTrace(st,6));
			t.printStackTrace();
		} catch (...) {
			Array<StackTraceElement> st;
			Throwable t;
			t.setStackTrace(captureStackTrace(st,6));
			t.printStackTrace();
		}
	}
	else {
		System.err.println("Terminated, no exception");
	}
	std::_Exit(EXIT_FAILURE);
}
class NullRef : extends Object {
} nullObject;

}

namespace lang {

Object& nullref = nullObject;

#ifdef BACKTRACE
void CallTrace::r() {
	tracePush(this);
}
CallTrace::~CallTrace() {
	CallTrace *c = tracePop();
	if (c != this) std::cerr << "stack mismach" << std::endl;
}
#endif

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
	return (message != null) ? (s + ": " + message) : s;
}
Throwable& Throwable::fillInStackTrace() {
	(void)captureStackTrace;
#ifdef BACKTRACE
	captureStack2(stackTrace);
#else
	captureStackTrace(stackTrace);
#endif
	return *this;
}
void Throwable::printStackTrace() const {TRACE;
	printStackTrace(System.err);
}
void Throwable::printStackTrace(io::PrintStream& s) const {TRACE;
	synchronized(s) {
	s.println("Exception in thread \"" + threadInfo + "\" " + this->toString());
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

Class::Class(const Object& o) : type(typeid(o)) {}
String Class::getSimpleName() const {TRACE;
	String simpleName = getSimpleBinaryName();
	if (simpleName.isEmpty()) { // top level class
		simpleName = getName();
		return simpleName.substring(simpleName.lastIndexOf(":")+1); // strip the package name
	}
	return simpleName;
}
String Class::getCanonicalName() const {TRACE;return getName();}
String Class::getTypeName(const std::type_info& type) {
	return demangle(type.name());
}

const Class Object::getClass() const {TRACE; return Class(*this);}

Object& Object::clone() const {TRACE;
	throw CloneNotSupportedException();
}
String Object::toString() const {
	return getClass().getName() + "@" + Integer::toHexString(hashCode());
}

String String::getTypeName(const std::type_info& type) {
	return demangle(type.name());
}

}
