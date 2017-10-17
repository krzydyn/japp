#ifndef __LANG_PROCESS_HPP
#define __LANG_PROCESS_HPP

#include <io/File.hpp>
#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>
#include <lang/Exception.hpp>
#include <util/HashMap.hpp>
/*
namespace io {
class OutputStream;
class InputStream;
}
*/
using namespace io;
using namespace util;
namespace lang {

class TimeUnit {
};

class Process : public Object {
public:
	virtual OutputStream& getOutputStream() = 0;
	virtual InputStream& getInputStream() = 0;
	virtual InputStream& getErrorStream() = 0;
	virtual int waitFor() = 0;
	boolean waitFor(long timeout, TimeUnit unit) {
	}
	virtual int exitValue() = 0;
	virtual void destroy() = 0;
	Process& destroyForcibly() {
		destroy();
		return *this;
	}
	boolean isAlive() {
		try {
			exitValue();
			return false;
		} catch(const IllegalThreadStateException& e) {
			return true;
		}
	}
};

class ProcessBuilder : public Object {
private:
	List<String>& cmd;
	File dir;
public:
	ProcessBuilder(List<String>& command) : cmd(command),dir(".") {}
	List<String>& command() { return cmd; }
	Map<String,String>& environment() {}
	const File& directory() { return dir; }
	ProcessBuilder& directory(const File& dir) {
		this->dir= dir;
		return *this;
	}
	Process& start() {
		return *((Process*)null);
	}
};

} //namespace lang

#endif
