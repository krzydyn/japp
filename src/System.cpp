#include <lang/System.hpp>
#include <io/FileInputStream.hpp>
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

const _System System;

} //namespace lang
