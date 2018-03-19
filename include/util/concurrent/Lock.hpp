#ifndef __UTIL_CONCURRENT_LOCK_HPP
#define __UTIL_CONCURRENT_LOCK_HPP

#include <lang/Object.hpp>

namespace util { namespace concurrent {

class TimeUnit {
};

interface Lock : Interface {
public:
	virtual void lock() = 0;
	virtual void lockInterruptibly() = 0;
	virtual boolean tryLock() = 0;
	virtual boolean tryLock(long time, TimeUnit unit) = 0;
	virtual void unlock() = 0;
	//Condition newCondition() = 0;
};

class ReentrantLock : implements Lock {
public:
	void lock();
	void lockInterruptibly();
	boolean tryLock();
	boolean tryLock(long time, TimeUnit unit);
	void unlock();
};

}}

#endif
