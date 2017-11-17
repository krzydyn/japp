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

class ProcessEnvironment final {
public:
	static const int MIN_NAME_LENGTH=1;
};

class Process : extends Object {
public:
	virtual OutputStream& getOutputStream() = 0;
	virtual InputStream& getInputStream() = 0;
	virtual InputStream& getErrorStream() = 0;
	virtual int waitFor() = 0;
	boolean waitFor(long timeout, TimeUnit unit) {
		return false;
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

class ProcessBuilder : extends Object {
private:
	List<String>& cmd;
	File dir;
	HashMap<String,String> env;
public:
	ProcessBuilder(List<String>& command) : cmd(command),dir(".") {}
	const List<String>& command() const { return cmd; }
	const Map<String,String>& environment() const {return env;}
	const File& directory() const { return dir; }
	ProcessBuilder& directory(File& dir) {
		this->dir= dir;
		return *this;
	}
	ProcessBuilder& environment(List<String> *envp) {
		if (envp == null) return *this;
		for (IteratorPtr<String> i=envp->iterator(); i->hasNext(); ) {
			String envstring = i->next();
			int eqlsign = envstring.indexOf('=', ProcessEnvironment::MIN_NAME_LENGTH);
			if (eqlsign != -1) env.put(envstring.substring(0,eqlsign), envstring.substring(eqlsign+1));
		}
		return *this;
	}
	Process& start();
};

} //namespace lang

#endif
