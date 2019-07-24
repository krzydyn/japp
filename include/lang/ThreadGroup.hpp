#ifndef __LANG_THREADGROUP_HPP
#define __LANG_THREADGROUP_HPP

#include <lang/Thread.hpp>
#include <io/PrintStream.hpp>
#include <util/Arrays.hpp>

namespace lang {

using namespace io;

class ThreadGroup : extends Object, implements Thread::UncaughtExceptionHandler {
friend class Thread;
private:
	ThreadGroup *parent;
	String name;
	int maxPriority;
	boolean destroyed;
	boolean daemon;
	boolean vmAllowSuspension;

	int nUnstartedThreads = 0;
	int nthreads;
	Array<Thread*> threads;
	int ngroups;
	Array<ThreadGroup*> groups;

	ThreadGroup() : parent(null) {
		name = "system";
		maxPriority = Thread::MAX_PRIORITY;
	}

	void add(Thread *t) {
		synchronized (*this) {
			if (destroyed) throw IllegalThreadStateException();
			if (threads.length == 0) {
				threads = Array<Thread*>(4);
			}
			else if (nthreads == threads.length) {
				threads = Arrays::copyOf(threads, nthreads * 2);
			}
			threads[nthreads] = t;
			++nthreads;
			--nUnstartedThreads;
		}
	}

	void add(ThreadGroup* g) {
		if (destroyed) throw IllegalThreadStateException();
		if (groups.length == 0) {
			groups = Array<ThreadGroup*>(4);
		}
		else if (ngroups == groups.length) {
			groups = Arrays::copyOf(groups, ngroups * 2);
		}
		groups[ngroups] = g;
		++ngroups;
	}
	void threadStartFailed(Thread *t) {
		synchronized (*this) {
			remove(t);
			nUnstartedThreads++;
		}
	}
	void threadTerminated(Thread *t) {
		synchronized (*this) {
			remove(t);
			if (nthreads == 0) {
				notifyAll();
			}
			if (daemon && (nthreads == 0) && (nUnstartedThreads == 0) && (ngroups == 0)) {
				destroy();
			}
		}
	}
	void remove(Thread *t) {
		synchronized (this) {
			if (destroyed) return ;
			for (int i = 0 ; i < nthreads ; i++) {
				if (threads[i] == t) {
					System::arraycopy(threads, i + 1, threads, i, --nthreads - i);
					threads[nthreads] = null;
					break;
				}
			}
		}
	}
	void remove(const ThreadGroup* g) {
		if (destroyed) return ;
		for (int i = 0 ; i < ngroups ; i++) {
			if (groups[i] == g) {
				ngroups -= 1;
				System::arraycopy(groups, i + 1, groups, i, ngroups - i);
				groups[ngroups] = null;
			}
		}
		if (nthreads == 0) notifyAll();
		if (daemon && nthreads == 0 && nUnstartedThreads == 0 && ngroups == 0)
			destroy();
	}

	void addUnstarted() {
		synchronized (*this) {
			if (destroyed) throw new IllegalThreadStateException();
			nUnstartedThreads++;
		}
	}

	int enumerate(Array<Thread*>& list, int n, boolean recurse) {
		if (destroyed) return 0;
		int nt = nthreads;
		if (nt > list.length - n) nt = list.length - n;
		for (int i = 0; i < nt; i++) {
			if (threads[i]->isAlive()) list[n++] = threads[i];
		}
		if (recurse) {
			for (int i = 0 ; i < ngroups; i++) {
				n = groups[i]->enumerate(list, n, true);
			}
		}
		return n;
	}
	void list(PrintStream& out, int indent) {
	}

protected:
	ThreadGroup(const String& name, ThreadGroup *p) : parent(p) {
		this->name = name;
		maxPriority = 1;
		vmAllowSuspension = false;
	}

public:
	ThreadGroup(const String& name) :
		ThreadGroup(Thread::currentThread().getThreadGroup(), name) { }

	ThreadGroup(ThreadGroup& par, const String& name) : parent(&par) {
		this->name = name;
		maxPriority = parent->maxPriority;
		vmAllowSuspension = parent->vmAllowSuspension;
		parent->add(this);
	}

	virtual const String& getName() const final { return name; }
	virtual const ThreadGroup* getParent() const final { return parent; }
	virtual int getMaxPriority() const final { return maxPriority; }
	virtual boolean isDaemon() const final { return daemon; }
	virtual boolean isDestroyed() const final { return destroyed; }
	virtual void setDaemon(boolean daemon) final {this->daemon = daemon;}
	virtual void setMaxPriority(int pri) final {
	}
	virtual boolean parentOf(const ThreadGroup* g) final {
		for (; g != null ; g = g->parent) {
			if (g == this) return true;
		}
		return false;
	}
	void checkAccess() {}
	int activeCount() {
		if (destroyed) return 0;
		int count = nthreads;
		for (int i = 0 ; i < ngroups; ++i) {
			count += groups[i]->activeCount();
		}
		return count;
	}
	int enumerate(Array<Thread*>& list) {
		checkAccess();
		return enumerate(list, 0, true);
	}
	int enumerate(Array<Thread*>& list, boolean recurse) {
		checkAccess();
		return enumerate(list, 0, recurse);
	}
	virtual void interrupt() final {
		for (int i = 0 ; i < nthreads ; ++i) {
			threads[i]->interrupt();
		}
		for (int i = 0 ; i < ngroups; ++i) {
			groups[i]->interrupt();
		}
	}
	virtual void destroy() final {
		if (destroyed || (nthreads > 0)) throw IllegalThreadStateException();
		int ngroupsSnap = ngroups;
		Array<ThreadGroup*> groupsSnap = groups;
		if (parent != null) {
			destroyed = true;
			ngroups = 0;
			nthreads = 0;
		}
		for (int i = 0 ; i < ngroupsSnap; ++i) {
			groupsSnap[i]->destroy();
		}
		if (parent != null) parent->remove(this);
	}
	void list() {
		list(System::out, 0);
	}
	void uncaughtException(const Thread& t, const Throwable& e) {
		if (parent != null) {
			parent->uncaughtException(t, e);
		}
		else {
			Thread::UncaughtExceptionHandler* ueh = Thread::getDefaultUncaughtExceptionHandler();
			if (ueh != null) {
				ueh->uncaughtException(t, e);
			}
			else {
				System::err.print("[G]Exception in thread \""+ t.getName() + "\" ");
				e.printStackTrace(System::err);
			}
		}
	}
	String toString() const {
		return getClass().getName() + "[name=" + getName() + ",maxpri=" + maxPriority + "]";
	}
};

} //namespace lang

#endif
