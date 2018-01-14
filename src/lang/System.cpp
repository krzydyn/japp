#include <lang/System.hpp>
#include <chrono>
#include <cstdlib> //std::getenv

using namespace std::chrono;

namespace {
auto nano_start = high_resolution_clock::now();
}

namespace lang {

void The_Log::format(const char *fmt, va_list& args) const {
	System.out.println(String::format(fmt,args));
}
const The_Log& The_Log::log(const String& s) const {
	System.out.println(s);
	return *this;
}
const The_Log& The_Log::log(const char *fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fmt,args);
	va_end(args);
	return *this;
}
const The_Log& The_Log::log(const String& fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fmt.intern().c_str(),args);
	va_end(args);
	return *this;
}

jlong The_System::currentTimeMillis() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
jlong The_System::nanoTime() {
	return duration_cast<nanoseconds>(high_resolution_clock::now()-nano_start).count();
}

const String The_System::getenv(const String& name) {
	const char *v = std::getenv(name.intern().c_str());
	if (v == null) return String();
	return v;
}

void The_System::arraycopy(const Object& src, int srcPos, Object& dest, int destPos, int length) {
}

} //namespace lang

#ifdef INSTRUMENTATION
/*
 * Instrumentation
 * compile with gcc '-finstrument-functions' flag
 */
__attribute__((no_instrument_function))
void __cyg_profile_func_enter(void *this_fn, void *call_site) {
}
__attribute__((no_instrument_function))
void __cyg_profile_func_exit(void *this_fn, void *call_site) {
}
#endif
