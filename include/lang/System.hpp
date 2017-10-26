#ifndef __LANG_SYSTEM_HPP
#define __LANG_SYSTEM_HPP

#include <lang/Object.hpp>
#include <io/InputStream.hpp>
#include <io/PrintStream.hpp>
#include <cstdlib>

namespace lang {

class The_System final {
public:
	The_System(){}
	The_System(const The_System& other) = delete;
	The_System(The_System&& other) = delete;
	The_System& operator=(const The_System& other) = delete;
	The_System& operator=(The_System&& other) = delete;
	//~The_System() = delete;

	static io::PrintStream& err;
	static io::PrintStream& out;
	static io::InputStream& in;

	static void setIn(io::InputStream& s) {The_System::in=s; }
	static void setOut(io::PrintStream& s) {The_System::out=s; }
	static void setErr(io::PrintStream& s) {The_System::err=s; }

	static jlong currentTimeMillis();
	static String getenv(const String& name);
	static String lineSeparator() {return "\n";}
	/**
	 * Terminates the currently running excutable by initiating its shutdown sequence.
	 */
	static void exit(int code);
	static void gc();
};

extern const The_System System;

} //namespace lang

#endif
