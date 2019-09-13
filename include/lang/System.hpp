#ifndef __LANG_SYSTEM_HPP
#define __LANG_SYSTEM_HPP

#include <lang/Runtime.hpp>
#include <io/InputStream.hpp>
#include <io/PrintStream.hpp>
#include <util/Properties.hpp>
#include <cstdlib>

namespace lang {

class System final : extends Object {
private:
	System() = delete;

	static Properties props;
	static String lineSep;
	static void arraycopy(const void *src, int srcPos, void *dest, int destPos, int length);

	static Properties initProperties(const Properties& props);

	static void checkKey(const String& key) {
		if (key == null_obj) {
			throw new NullPointerException("key can't be null");
		}
		if (key.equals("")) {
			throw new IllegalArgumentException("key can't be empty");
		}
	}
public:

	static void static_init();
	static io::PrintStream& err;
	static io::PrintStream& out;
	static io::InputStream& in;

	static void setIn(io::InputStream& s) {System::in=s; }
	static void setOut(io::PrintStream& s) {System::out=s; }
	static void setErr(io::PrintStream& s) {System::err=s; }

	static long currentTimeMillis();
	/**
	 * This method can only be used to measure elapsed time and is
	 * not related to any other notion of system or wall-clock time.
	 */
	static long nanoTime();
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
		checkKey(key);
		return props.getProperty(key);
	}
	static const String& getProperty(const String& key, const String& def) {
		checkKey(key);
		return props.getProperty(key, def);
	}
	static const String& setProperty(const String& key, const String& value) {
		checkKey(key);
		return props.setProperty(key, value);
	}
	static String clearProperty(String key) {
		checkKey(key);
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

class Logger : extends Object {
private:
	static boolean release;
	void format(const char *fn, unsigned ln, int level, const char *fmt, va_list& args) const;

public:
	const Logger& error(const char *fn, unsigned ln, const String& s) const {
		return error(fn, ln, "%s", s.cstr());
	}
	const Logger& error(const char *fn, unsigned ln, const char *fmt...) const;

	const Logger& warn(const char *fn, unsigned ln, const String& s) const {
		return warn(fn, ln, "%s", s.cstr());
	}
	const Logger& warn(const char *fn, unsigned ln, const char *fmt...) const;

	const Logger& debug(const char *fn, unsigned ln, const String& s) const {
		return debug(fn, ln, "%s", s.cstr());
	}
	const Logger& debug(const char *fn, unsigned ln, const char *fmt...) const;

	const Logger& info(const char *fn, unsigned ln, const String& s) const {
		return info(fn, ln, "%s", s.cstr());
	}
	const Logger& info(const char *fn, unsigned ln, const char *fmt...) const;

	const Logger& notice(const char *fn, unsigned ln, const String& s) const {
		return notice(fn, ln, "%s", s.cstr());
	}
	const Logger& notice(const char *fn, unsigned ln, const char *fmt...) const;
};
extern const Logger SystemLog;

#define LOGE(fmt...) SystemLog.error(__FILE__,__LINE__,fmt)
#define LOGW(fmt...) SystemLog.warn(__FILE__,__LINE__,fmt)
#define LOGD(fmt...) SystemLog.debug(__FILE__,__LINE__,fmt)
#define LOGI(fmt...) SystemLog.info(__FILE__,__LINE__,fmt)
#define LOGN(fmt...) SystemLog.notice(__FILE__,__LINE__,fmt)

#define LOGM(fmt...) SystemLog.method(__FILE__,__LINE__,__FUNCTION__,fmt)

} //namespace lang

#endif
