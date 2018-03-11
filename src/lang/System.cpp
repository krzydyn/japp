#include <lang/System.hpp>
#include <chrono>
#include <cstdlib> //std::getenv
#include <time.h>

using namespace std::chrono;

namespace {
auto nano_start = high_resolution_clock::now();
}

namespace lang {

void Logger::format(const char *fn, unsigned ln, int level, const char *fmt, va_list& args) const {
	jlong jtm = System.currentTimeMillis();
	int r = 0;
	time_t t = (time_t)(jtm/1000);
	if (jtm >= 0) r = (int)(jtm%1000);
	else if (jtm < 0) r = (int)((-jtm)%1000);
	char buf[30];
	struct tm stm;
	gmtime_r(&t, &stm);
	strftime (buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime_r(&t, &stm));
	System.out.printf("%s.%03llu %s(%u): ", buf, r, fn, ln);
	System.out.println(String::format(fmt, args));
}

const Logger& Logger::error(const char *fn, unsigned ln, const String& s) const {
	return debug(fn, ln, "%s", s.cstr());
}
const Logger& Logger::error(const char *fn, unsigned ln, const char *fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 2, fmt, args);
	va_end(args);
	return *this;
}
const Logger& Logger::error(const char *fn, unsigned ln, const String& fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 2, fmt.cstr(), args);
	va_end(args);
	return *this;
}
const Logger& Logger::debug(const char *fn, unsigned ln, const String& s) const {
	return debug(fn, ln, "%s", s.cstr());
}
const Logger& Logger::debug(const char *fn, unsigned ln, const char *fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 1, fmt, args);
	va_end(args);
	return *this;
}
const Logger& Logger::debug(const char *fn, unsigned ln, const String& fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 1, fmt.cstr(), args);
	va_end(args);
	return *this;
}
const Logger& Logger::info(const char *fn, unsigned ln, const String& s) const {
	return info(fn, ln, "%s", s.cstr());
}
const Logger& Logger::info(const char *fn, unsigned ln, const char *fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 0, fmt, args);
	va_end(args);
	return *this;
}
const Logger& Logger::info(const char *fn, unsigned ln, const String& fmt...) const {
	va_list args;
	va_start(args, fmt);
	format(fn, ln, 0, fmt.cstr(), args);
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
	const char *v = std::getenv(name.cstr());
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
