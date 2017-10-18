#include <lang/Process.hpp>
#include <lang/System.hpp>
#include <lang/Runtime.hpp>
#include <io/File.hpp>
#include <io/FileInputStream.hpp>
#include <io/FileOutputStream.hpp>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

namespace lang {
void Shutdown::exit(int status) {
	std::exit(status);
}
static io::FileInputStream std_fin(std::cin);
static io::FileOutputStream std_fout(std::cout);
static io::FileOutputStream std_ferr(std::cerr);
static io::PrintStream std_out(std_fout);
static io::PrintStream std_err(std_ferr);

io::InputStream& _System::in = std_fin;
io::PrintStream& _System::out = std_out;
io::PrintStream& _System::err = std_err;

jlong _System::currentTimeMillis() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

String _System::getenv(const String& name) {
	const char *v = std::getenv(name.intern().c_str());
	if (v == null) return String();
	return v;
}

void _System::exit(int code) { Runtime::getRuntime().exit(code); }
void _System::gc() { Runtime::getRuntime().gc(); }
const _System System;

Process& ProcessBuilder::start() {
	Process *p = null;
	return *p;
}

Runtime Runtime::currentRuntime;

} //namespace lang

namespace io {
const String File::separator = "/";
}
