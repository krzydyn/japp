#include <util/concurrent/Lock.hpp>

namespace util { namespace concurrent {

void ReentrantLock::lock() {
}
void ReentrantLock::lockInterruptibly() {
}
boolean ReentrantLock::tryLock() {
	return false;
}
boolean ReentrantLock::tryLock(long time, TimeUnit unit) {
	return false;
}
void ReentrantLock::unlock() {
}

}}
