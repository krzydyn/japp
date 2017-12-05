#ifndef __LANG_PROCESS_HPP
#define __LANG_PROCESS_HPP

#include <io/File.hpp>
#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>
#include <lang/Exception.hpp>
#include <util/HashMap.hpp>

namespace lang {

class TimeUnit {
};

class ProcessEnvironment final : Interface {
private:
	static util::HashMap<String,String> theEnvironment;
public:
	static const int MIN_NAME_LENGTH=1;

	static const String& getenv(String name) {
	   return theEnvironment.get(name);
	}
	static const util::Map<String,String>& getenv() {
		return theEnvironment;
	}
};

class Process : extends Object {
public:
	virtual io::OutputStream& getOutputStream() = 0;
	virtual io::InputStream& getInputStream() = 0;
	virtual io::InputStream& getErrorStream() = 0;
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
	util::List<String>& cmd;
	io::File dir;
	util::HashMap<String,String> env;

public:
	ProcessBuilder(util::List<String>& command) : cmd(command),dir(".") {}
	const util::List<String>& command() const { return cmd; }
	const util::Map<String,String>& environment() const {return env;}
	const io::File& directory() const { return dir; }
	ProcessBuilder& directory(io::File& dir) {
		this->dir= dir;
		return *this;
	}
	ProcessBuilder& environment(util::List<String> *envp) {
		if (envp == null) return *this;
		for (SharedIterator<String> i=envp->iterator(); i->hasNext(); ) {
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
