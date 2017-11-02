#include <lang/Thread.hpp>

namespace lang {

long Thread::threadSeqNumber = 0;
void Thread::start() {
	this->tid=threadSeqNumber;
	++threadSeqNumber;

	//this->thread = std::thread(&Thread::run, std::ref(*this));
	this->thread = std::thread([=] {run();});
}

} //namespace lang
