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

// addr2line for *.so
//https://stackoverflow.com/questions/7556045/how-to-map-function-address-to-function-in-so-files
// addr2line -j .text -e libtst.so 0x26887
// where 0x26887 is 0xb77dc887(fun2 addr+offset)-0xb77b6000 (lib starting addr)
//
// http://blog.bigpixel.ro/2010/09/stack-unwinding-stack-trace-with-gcc/
// readelf --debug-dump=decodedline  a.out

namespace {
#define CALLTRACE_SIZE 2048
CallTrace *calltrace[CALLTRACE_SIZE];
unsigned calltrace_size = 0;
void tracePush(CallTrace *c) {
	if (calltrace_size < CALLTRACE_SIZE)
		calltrace[calltrace_size] = c;
	++calltrace_size;
}
CallTrace *tracePop() {
	if (calltrace_size == 0) return null;
	--calltrace_size;
	if (calltrace_size < CALLTRACE_SIZE)
		return calltrace[calltrace_size];
	return null;
}
unsigned traceSize() { return calltrace_size; }

std::string demangle(const std::string& name) {
#ifdef __GNUG__ // gnu C++ compiler
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr< char, decltype(&std::free) > ptr(
		__cxxabiv1::__cxa_demangle(name.c_str(), null, &len, &status), &std::free );
	return status == 0 ? ptr.get() : name;
#else
	return name;
#endif
}
String getSimpleBinaryName() { return ""; }
#ifdef __APPLE__
// 0   threads                             0x000000010b84db25 _ZN4lang9Throwable12captureStackEi + 211
StackTraceElement parseStackEntry(const std::string& s) {
	int posOp = s.find("0x");
	posOp = s.find(' ', posOp);
	int posCl = s.find('+',posOp);
	if (posOp != std::string::npos && posCl != std::string::npos) {
		posOp += 1; posCl -= 1;
		std::string func = demangle(s.substr(posOp,posCl-posOp));
		//std::printf("demangling: '%s'\n", s.substr(posOp,posCl-posOp).c_str());
		return StackTraceElement(func + s.substr(posCl),"",0);
	}
	return StackTraceElement(s,"",0);
}
#elif __linux__
// ./build/test/threads (mangled_name+0x24) [0x8c50]
StackTraceElement parseStackEntry(const std::string& s) {
	int posOp = s.find('(');
	int posCl = s.find('+',posOp);
	if (posOp != std::string::npos && posCl != std::string::npos) {
		posOp += 1;
		//std::printf("demangling: '%s'\n", s.substr(posOp,posCl-posOp).c_str());
		return StackTraceElement(s.substr(0,posOp)+demangle(s.substr(posOp,posCl-posOp))+s.substr(posCl),"",0);
	}
	return StackTraceElement(s,"",0);
}
#else
StackTraceElement parseStackEntry(const std::string& s) {
	return StackTraceElement(s,"",0);
}
#endif

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
	//std::printf("removed bt[%d]: f='%s' @ (%s:%u)\n",traceSize(),c->func,c->file,c->line);
}

void Throwable::captureStack(int depth) { TRACE;
   	void *array[depth];
	int got = ::backtrace(array, depth);
	if (got <= 2) return ;
	//better backtrace_symbols
	//http://cairo.sourcearchive.com/documentation/1.9.4/backtrace-symbols_8c-source.html
	char **strings = ::backtrace_symbols(array, got);
	got -= 2;
	stackTrace = Array<StackTraceElement>(got);
	for (int i = 0; i < got; ++i) {
		//std::printf("bt[%d]: %s\n", i, strings[i + 2]);
		stackTrace[i] = parseStackEntry(strings[i + 2]);
	}
	::free (strings);
}
void Throwable::captureStack2() {
	std::cout << "capture2 ..." << std::endl;
	stackTrace = Array<StackTraceElement>(traceSize());
	for (int i = stackTrace.length; i > 0; ) {
		--i;
		CallTrace *ct = calltrace[i];
		if (ct == null) continue;
		//std::printf("bt[%d]: f='%s'  @ '%s:%d'\n", i, ct->func,ct->file,ct->line);
		stackTrace[i] = StackTraceElement(ct->func,ct->file,ct->line);
	}
	std::cout << "capture2 done" << std::endl;
}
void Throwable::printStackTrace() const {TRACE;
	printStackTrace(System.err);	
}
void Throwable::printStackTrace(const io::PrintStream& s) const {TRACE;
	//s.println((const Object&)(*this));
	//s.println(this->toString());
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
