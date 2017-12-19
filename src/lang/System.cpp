#include <lang/Math.hpp>
#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileInputStream.hpp>
#include <io/FileOutputStream.hpp>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

namespace {
io::InputStream& std_in() {
	static io::FileInputStream stream(std::cin);
	return stream;
}
io::PrintStream& std_out() {
	static io::FileOutputStream fstream(std::cout);
	static io::PrintStream stream(fstream);
	std::cout << "std_out init" << std::endl;
	return stream;
}
io::PrintStream& std_err() {
	static io::FileOutputStream fstream(std::cerr);
	static io::PrintStream stream(fstream);
	std::cout << "std_err init" << std::endl;
	return stream;
}

HashMap<String,String> env;
auto nano_start = high_resolution_clock::now();
}

namespace lang {
const The_System System;
const The_Math Math;

Properties The_System::props;
io::InputStream& The_System::in = std_in();
io::PrintStream& The_System::out = std_out();
io::PrintStream& The_System::err = std_err();

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
const util::Map<String,String>& The_System::getenv() {
	//return ProcessEnvironment.getenv();
	return env;
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
