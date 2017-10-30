#include <lang/Math.hpp>
#include <lang/Process.hpp>
#include <lang/System.hpp>
#include <lang/Runtime.hpp>
#include <io/File.hpp>
#include <io/FileInputStream.hpp>
#include <io/FileOutputStream.hpp>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

namespace {
io::FileInputStream std_fin(std::cin);
io::FileOutputStream std_fout(std::cout);
io::FileOutputStream std_ferr(std::cerr);
io::PrintStream std_out(std_fout);
io::PrintStream std_err(std_ferr);
}
namespace lang {
const The_System System;
const The_Math Math;

void Shutdown::exit(int status) {
	std::exit(status);
}

Properties The_System::props;
io::InputStream& The_System::in = std_fin;
io::PrintStream& The_System::out = std_out;
io::PrintStream& The_System::err = std_err;

jlong The_System::currentTimeMillis() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

const String The_System::getenv(const String& name) {
	const char *v = std::getenv(name.intern().c_str());
	if (v == null) return String();
	return v;
}

void The_System::exit(int code) { Runtime::getRuntime().exit(code); }
void The_System::gc() { Runtime::getRuntime().gc(); }

Process& ProcessBuilder::start() {
	Process *p = null;
	return *p;
}

Runtime Runtime::currentRuntime;

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
