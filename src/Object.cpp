#include <lang/Object.hpp>
#include <lang/Math.hpp>
#include <lang/System.hpp>
#include <iomanip>
#include <sstream>

#include <execinfo.h>

#ifdef __GNUG__ // gnu C++ compiler
#include <memory>
#include <cxxabi.h>
#endif

namespace {
std::string demangle(const char* mangled_name) {
#ifdef __GNUG__ // gnu C++ compiler
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr< char, decltype(&std::free) > ptr(
		__cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free );
	return ptr.get();
#else
	return name;
#endif
}
String getSimpleBinaryName() { return ""; }
}

namespace lang {

void Throwable::captureStack(int depth) {
   	void *array[depth];
	int got = ::backtrace(array, depth);
	std::cout << "got " << got << std::endl;
	if (got <= 0) return ;
	stackTrace = new Array<StackTraceElement>(got);
	char **strings = ::backtrace_symbols(array, got);
	for (int tn = 0; tn < got; ++tn) {
		(*stackTrace)[tn] = StackTraceElement((const char *)strings[tn],"",0);
	}
	::free (strings);
	std::cout << "(*stackTrace).length = " << (*stackTrace).length << std::endl;
	for (int i=0; i < got; ++i)
		System.out.println((*stackTrace)[i].getMethodName());
	std::cout << "capture done" << std::endl;
}

Class::Class(const Object& o) : type(typeid(o)) {}
String Class::getName() const { return demangle(type.name()); }
String Class::getSimpleName() const {
	String simpleName = getSimpleBinaryName();
	if (simpleName.isEmpty()) { // top level class
		simpleName = getName();
		return simpleName.substring(simpleName.lastIndexOf(":")+1); // strip the package name
	}
	return simpleName;
}
String Class::getCanonicalName() const { return getName(); }
std::string Class::nameOf(const std::type_info& type) {
	return demangle(type.name());
}
//String Class::nameOf(const std::type_info& type) { return demangle(type.name()); }

Object& Object::clone() const {
	throw CloneNotSupportedException();
}
String Object::toString() const {
	return getClass().getName() + "@" + Integer::toHexString(hashCode());
}

String Integer::toHexString(long v) {
	std::stringstream stream;
	stream << std::hex << (unsigned long)v;
	return stream.str();
}


_Math Math;
}
