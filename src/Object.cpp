#include <lang/String.hpp>
#include <lang/Math.hpp>
#include <iomanip>
#include <sstream>

#ifdef __GNUG__ // gnu C++ compiler
#include <memory>
#include <cxxabi.h>
namespace {
std::string demangle(const char* mangled_name) {
	std::size_t len = 0;
	int status = 0;
	std::unique_ptr< char, decltype(&std::free) > ptr(
		__cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free );
	return ptr.get();
}
}
#else
namespace {
std::string demangle(const char* name) { return name; }
}
#endif // _GNUG_

namespace {
String getSimpleBinaryName() { return ""; }
}

namespace lang {

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


static ObjectInfo info;
const Class Object::getClass() const { return Class(typeid(*this),info); }
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
