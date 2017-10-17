#ifndef __LANG_RUNTIME_HPP
#define __LANG_RUNTIME_HPP

namespace lang {

class Process : public Object {
public:
	OutputStream& getOutputStream() = 0;
	InputStream& getInputStream() = 0;
	InputStream& getErrorStream() = 0;
	int waitFor() = 0;
	boolean waitFor(long timeout, TimeUnit unit) {
	}
	int exitValue() = 0;
	void destroy() = 0;
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
	List<String>& command;
	File directory;
public:
	ProcessBuilder(List<String>& command) {
		this->command = command;
	}
	List<String>& command() { return command; }
	Map<String,String> environment() {}
	File directory() { return directory; }
	ProcessBuilder& directory(File directory) {
		this->directory = directory;
		return *this;
	}
	Process start() {
		Process p;
		return p;
	}
};

} //namespace lang


#endif
