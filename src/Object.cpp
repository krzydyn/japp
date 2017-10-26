#include <lang/Object.hpp>
#include <lang/System.hpp>
#include <iomanip>
#include <sstream>
#include <vector>

#include <execinfo.h>

#ifdef __GNUG__ // gnu C++ compiler
#include <memory>
#include <cxxabi.h>
#endif

namespace {
#define CALLTRACE_SIZE 1000
CallTrace *calltrace[CALLTRACE_SIZE];
unsigned calltrace_size = 0;
void tracePush(CallTrace *c) {
	calltrace[calltrace_size] = c;
	++calltrace_size;
}
CallTrace *tracePop() {
	if (calltrace_size == 0) return null;
	--calltrace_size;
	return calltrace[calltrace_size];
}
unsigned traceSize() { return calltrace_size; }

std::string demangle(const std::string& name) {
#ifdef __GNUG__ // gnu C++ compiler
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr< char, decltype(&std::free) > ptr(
		__cxxabiv1::__cxa_demangle(name.c_str(), nullptr, &len, &status), &std::free );
	return status == 0 ? ptr.get() : name;
#else
	return name;
#endif
}
String getSimpleBinaryName() { return ""; }
}

namespace lang {

void CallTrace::r() {
	int i = traceSize();
	tracePush(this);
	//std::printf("added bt[%d]: f='%s' @ (%s:%u)\n",i,func,file,line);
}
CallTrace::~CallTrace() {
	CallTrace *c = tracePop();
	if (c != this) std::cerr << "stack mismach" << std::endl;
	int i = traceSize();
	//std::printf("removed bt[%d]: f='%s' @ (%s:%u)\n",i,c->func,c->file,c->line);
}

void Throwable::captureStack(int depth) { TRACE;
   	void *array[depth];
	int got = ::backtrace(array, depth);
	if (got <= 0) return ;
	stackTrace = Array<StackTraceElement>(got);
	char **strings = ::backtrace_symbols(array, got);
	for (int i = 0; i < got; ++i) {
		//std::printf("bt[%d]: %s\n", tn, strings[tn]);
		const std::string& s(strings[i]);
		int posOp = s.find('(');
		int posCl = s.find('+',posOp);
		if (posOp != std::string::npos && posCl != std::string::npos) {
			posOp += 1;
			//std::printf("demangling: '%s'\n", s.substr(posOp,posCl-posOp).c_str());
			stackTrace[i] = StackTraceElement(s.substr(0,posOp)+demangle(s.substr(posOp,posCl-posOp))+s.substr(posCl),"",0);
		}
		else {
			stackTrace[i] = StackTraceElement(s,"",0);
		}
	}
	::free (strings);
	std::cout << "stackTrace.length = " << stackTrace.length << std::endl;
	for (int i=0; i < stackTrace.length; ++i)
		System.out.println(stackTrace[i].getMethodName());
	std::cout << "capture done" << std::endl;
}
void Throwable::captureStack2() {
	std::cout << "capture2 ..." << std::endl;
	stackTrace = Array<StackTraceElement>(traceSize());
	for (int i = stackTrace.length; i > 0; ) {
		--i;
		CallTrace *ct = calltrace[i];
		//std::printf("bt[%d]: f='%s'  @ '%s:%d'\n", i, ct->func,ct->file,ct->line);
		stackTrace[i] = StackTraceElement(ct->func,ct->file,ct->line);
	}
	std::cout << "capture2 done" << std::endl;
}
void Throwable::printStackTrace() {TRACE;
	printStackTrace(System.err);	
}
void Throwable::printStackTrace(const io::PrintStream& s) {TRACE;
	//s.println((const Object&)(*this));
	s.println(*this);
	for (int i=0; i < stackTrace.length; ++i) {
		s.print("\tat ");
		s.println(stackTrace[i].toString());
	}
}

Class::Class(const Object& o) : type(typeid(o)) {}
String Class::getName() const {return demangle(type.name()); }
String Class::getSimpleName() const {TRACE;
	String simpleName = getSimpleBinaryName();
	if (simpleName.isEmpty()) { // top level class
		simpleName = getName();
		return simpleName.substring(simpleName.lastIndexOf(":")+1); // strip the package name
	}
	return simpleName;
}
String Class::getCanonicalName() const {TRACE;return getName();}
std::string Class::nameOf(const std::type_info& type) {TRACE;
	return demangle(type.name());
}
//String Class::nameOf(const std::type_info& type) { return demangle(type.name()); }

Object& Object::clone() const {TRACE;
	throw CloneNotSupportedException();
}
String Object::toString() const {TRACE;
	return getClass().getName() + "@" + Integer::toHexString(hashCode());
}

String Integer::toHexString(long v) {TRACE;
	std::stringstream stream;
	stream << std::hex << (unsigned long)v;
	return stream.str();
}

}
