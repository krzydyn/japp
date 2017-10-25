#include <lang/Object.hpp>
#include <lang/Math.hpp>
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
std::vector<CallTrace*> calltrace;

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

CallTrace::CallTrace(const char *func, const char *file, int ln) : func(func), file(file), ln(ln) {
	calltrace.push_back(this);
}
CallTrace::~CallTrace() {
	CallTrace *c = calltrace.back();
	if (c != this) std::cerr << "stack mismach" << std::endl;
	calltrace.pop_back();
}

void Throwable::captureStack(int depth) {
	TRACE;
   	void *array[depth];
	int got = ::backtrace(array, depth);
	if (got <= 0) return ;
	stackTrace = Array<StackTraceElement>(got);
	char **strings = ::backtrace_symbols(array, got);
	for (int tn = 0; tn < got; ++tn) {
		//std::printf("bt[%d]: %s\n", tn, strings[tn]);
		const std::string& s(strings[tn]);
		int posOp = s.find('(');
		int posCl = s.find('+',posOp);
		if (posOp != std::string::npos && posCl != std::string::npos) {
			posOp += 1;
			//std::printf("demangling: '%s'\n", s.substr(posOp,posCl-posOp).c_str());
			stackTrace[tn] = StackTraceElement(s.substr(0,posOp)+demangle(s.substr(posOp,posCl-posOp))+s.substr(posCl),"",0);
		}
		else {
			stackTrace[tn] = StackTraceElement(s,"",0);
		}
	}
	::free (strings);
	std::cout << "stackTrace.length = " << stackTrace.length << std::endl;
	for (int i=0; i < got; ++i)
		System.out.println(stackTrace[i].getMethodName());
	std::cout << "capture done" << std::endl;
}

Class::Class(const Object& o) : type(typeid(o)) {TRACE;}
String Class::getName() const {TRACE; return demangle(type.name()); }
String Class::getSimpleName() const {
	TRACE;
	String simpleName = getSimpleBinaryName();
	if (simpleName.isEmpty()) { // top level class
		simpleName = getName();
		return simpleName.substring(simpleName.lastIndexOf(":")+1); // strip the package name
	}
	return simpleName;
}
String Class::getCanonicalName() const {TRACE;return getName();}
std::string Class::nameOf(const std::type_info& type) {
	TRACE;
	return demangle(type.name());
}
//String Class::nameOf(const std::type_info& type) { return demangle(type.name()); }

Object& Object::clone() const {
	TRACE;
	throw CloneNotSupportedException();
}
String Object::toString() const {
	TRACE;
	return getClass().getName() + "@" + Integer::toHexString(hashCode());
}

String Integer::toHexString(long v) {
	TRACE;
	std::stringstream stream;
	stream << std::hex << (unsigned long)v;
	return stream.str();
}


_Math Math;
}
