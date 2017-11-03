#include <lang/System.hpp>
#include <lang/Thread.hpp>
#include <chrono>

namespace lang {

long Thread::threadSeqNumber = 0;

void Thread::sleep(long millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Thread::setNativeName(const String& name) {
	pthread_setname_np(thread.native_handle(), name.intern().c_str());
}
void Thread::setName(const String& name) {
	if (name == null) {
		throw NullPointerException("name cannot be null");
	}
	this->name=name;
	if (threadStatus != 0) {
		setNativeName(name);
	}
}
void Thread::runHelper() {
	//TODO use mutex, condition... for synchronization
	while (threadStatus == NEW) sleep(10);
	if (threadStatus != RUNNABLE) {
		return ;
	}
	setNativeName(name);
	System.out.println(String("running thread ")+name);
	run();
	System.out.println(String("terminated thread ")+name);
	threadStatus = TERMINATED;
}

void Thread::start() {
	if (threadStatus != NEW) {
		throw IllegalThreadStateException();
	}
	this->tid=threadSeqNumber;
	++threadSeqNumber;
	
	this->thread = std::thread([=] {runHelper();});
	threadStatus = RUNNABLE;
}

} //namespace lang
