#ifndef __LANG_SYSTEM_HPP
#define __LANG_SYSTEM_HPP

#include <lang/Object.hpp>
#include <io/InputStream.hpp>
#include <io/PrintStream.hpp>

namespace lang {

class _System {
public:
	_System() {}
	_System(const _System& other) = delete;
	_System(_System&& other) = delete;
	_System& operator=(const _System& other) = delete;
	_System& operator=(_System&& other) = delete;

	static io::PrintStream& err;
	static io::PrintStream& out;
	static io::InputStream& in;

	static void setIn(io::InputStream& s) { _System::in=s; }
	static void setOut(io::PrintStream& s) { _System::out=s; }
	static void setErr(io::PrintStream& s) { _System::err=s; }

	static jlong currentTimeMillis();
	static String getenv(const String& name);
	static String lineSeparator() {return "\n";}
	static void exit(int code) { ::exit(code); }
	static void gc() { }
};

extern const _System System;

} //namespace lang

#endif
