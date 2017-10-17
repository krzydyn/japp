#include <lang/System.hpp>
#include <lang/Runtime.hpp>
#include <io/FileInputStream.hpp>
#include <io/File.hpp>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

namespace lang {
static io::FileInputStream std_in(std::cin);
static io::PrintStream std_out(std::cout);
static io::PrintStream std_err(std::cerr);

io::InputStream& _System::in = std_in;
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

} //namespace lang

namespace io {
const String File::separator = "/";
}
