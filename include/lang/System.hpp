#ifndef __LANG_SYSTEM_HPP
#define __LANG_SYSTEM_HPP

#include <lang/Runtime.hpp>
#include <io/InputStream.hpp>
#include <io/PrintStream.hpp>
#include <util/Properties.hpp>
#include <cstdlib>

namespace lang {

class The_System final : Interface {
private:
	static Properties props;
	static String lineSep;
	static void arraycopy(const void *src, int srcPos, void *dest, int destPos, int length);
public:
	The_System(){}
	~The_System(){}

	static void static_init();
	static io::PrintStream& err;
	static io::PrintStream& out;
	static io::InputStream& in;

	static void setIn(io::InputStream& s) {The_System::in=s; }
	static void setOut(io::PrintStream& s) {The_System::out=s; }
	static void setErr(io::PrintStream& s) {The_System::err=s; }

	static jlong currentTimeMillis();
	/**
	 * This method can only be used to measure elapsed time and is
	 * not related to any other notion of system or wall-clock time.
	 */
	static jlong nanoTime();
	static void arraycopy(const Object& src, int srcPos, Object& dest, int destPos, int length);
	template<class T>
	static void arraycopy(const Array<T>& src, int srcPos, Array<T>& dest, int destPos, int length) {
		//arraycopy(&src.a, srcPos*sizeof(T), dest.a, destPos*sizeof(T), length*sizeof(T));
		for (int i=0; i < length; ++i) dest[destPos + i] = src[srcPos + i];
	}
	static int identityHashCode(const Object& o) {
		throw UnsupportedOperationException();
	}
	static const Properties& getProperties() {return props;}
	static const String lineSeparator() {return lineSep;}
	static const String& getProperty(const String& key) {
		return props.getProperty(key);
	}
	static const String& getProperty(const String& key, const String& def) {
		return props.getProperty(key, def);
	}
	static const String& setProperty(const String& key, const String& value) {
		return props.setProperty(key, value);
	}
	static String clearProperty(String key) {
		return props.remove(key);
	}
	static const String getenv(const String& name);
	static const util::Map<String,String>& getenv();
	/**
	 * Terminates the currently running excutable by initiating its shutdown sequence.
	 */
	static void exit(int code) { Runtime::getRuntime().exit(code); }
	static void gc() { Runtime::getRuntime().gc(); }
	static void runFinalization() {
		Runtime::getRuntime().runFinalization();
	}
	static void load(String filename) {
		throw UnsupportedOperationException();
	}
	static void loadLibrary(String libname) {
		throw UnsupportedOperationException();
	}
	static String mapLibraryName(String libname) {
		throw UnsupportedOperationException();
	}
};

extern const The_System System;

} //namespace lang

#endif
