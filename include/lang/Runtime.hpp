#ifndef __LANG_RUNTIME_HPP
#define __LANG_RUNTIME_HPP

#include <lang/Thread.hpp>

namespace lang {

class Runtime : public Object {
private:
	static Runtime currentRuntime;
public:
	static Runtime& getRuntime() { return currentRuntime; }
	void exit(int status) {Shutdown.exit(status);}
	void halt(int status) {Shutdown.halt(status);}
	void addShutdownHook(Thread hook) { }
	boolean removeShutdownHook(Thread hook) {}
	Process exec(String command) {
		return exec(command, null, null);
	}
	Process exec(String command, String[] envp) {
		return exec(command, envp, null);
	}
	Process exec(String command, String[] envp, File dir) {
		if (command.length() == 0)
			throw new IllegalArgumentException("Empty command");
		StringTokenizer st = new StringTokenizer(command);
		String[] cmdarray = new String[st.countTokens()];
		for (int i = 0; st.hasMoreTokens(); ++i)
			cmdarray[i] = st.nextToken();
		return exec(cmdarray, envp, dir);
	}
	Process exec(String cmdarray[]) {
		return exec(cmdarray, null, null);
	}
	Process exec(String[] cmdarray, String[] envp) {
		return exec(cmdarray, envp, null);
	}
	Process exec(String[] cmdarray, String[] envp, File dir) {
		return new ProcessBuilder(cmdarray)
			.environment(envp)
			.directory(dir)
			.start();
    }
	int availableProcessors() { return 0; }
	long freeMemory() {return 0;}
	long totalMemory() {return 0;}
	long maxMemory() {return 0;}
	void gc() {}
	void runFinalization() {}
	void traceInstructions(boolean on) {}
	void traceMethodCalls(boolean on) {}
	void load(String filename) {}
	void loadLibrary(String libname) {}
};

} //namespace lang


#endif
