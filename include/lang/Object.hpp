#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <mutex>

#define boolean bool
#define null nullptr
#define interface class
#define extends public
#define implements virtual public

using jint=long;
using jlong=long long;

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) a ## b
#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)

namespace lang {

class Class;
class Object;
class String;

extern Object& nullref;

//#define TRACE CallTrace UNIQUE_NAME(the_calltrace)(__FUNCTION__, __FILE__,__LINE__);UNIQUE_NAME(the_calltrace).r()

#ifndef TRACE
#define TRACE
#undef BACKTRACE
#else
#define BACKTRACE
class CallTrace {
public:
	const char *func;
	const char *file;
	unsigned line;
	CallTrace(const char *func, const char *file,unsigned line):func(func),file(file),line(line){}
	void r();
	~CallTrace();
};
#endif

class Interface {
protected:
	Interface(const Interface& other) = delete;
	Interface(Interface&& other) = delete;
	Interface& operator=(const Interface& other) = delete;
	Interface& operator=(Interface&& other) = delete;
	virtual ~Interface() {}
	Interface() {}
};

class Object {
	friend class Lock;
private:
	std::recursive_mutex *mtx=null;
protected:
	virtual void finalize() {}
	virtual Object& clone() const;
public:
	Object(const Object& other) {}
	Object(Object&& other) {mtx = std::move(other.mtx);}
	Object& operator=(const Object& other) {return *this;}
	Object& operator=(Object&& other) {mtx = std::move(other.mtx);return *this;}
	virtual ~Object() {
		if (mtx) delete mtx;
	}
	Object() {}
	virtual const Class getClass() const;
	virtual long hashCode() const {return (long)this;}
	virtual long hashCode() {return ((const Object*)this)->hashCode();}

	virtual boolean equals(const Object& obj) const {return this == &obj;}
	virtual String toString() const;
	virtual void notify() {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final {wait(0); }

	boolean operator==(const std::nullptr_t&) const {return this == null; }
	boolean operator!=(const std::nullptr_t&) const {return this != null; }

	//virtual boolean operator==(const Object& obj) const final {return equals(obj); }
	//virtual boolean operator!=(const Object& obj) const final {return !equals(obj); }
	//virtual boolean operator==(const void *ptr) const {return ptr == this; }
	//virtual boolean operator!=(const void *ptr) const {return ptr != this; }

	class Lock {
	private:
		const Object& obj;
		boolean locked=true;
	public:
		Lock(const Object& o) : obj(o) {
			if (obj.mtx == null) {
				const_cast<Object&>(obj).mtx = new std::recursive_mutex;
			}
			obj.mtx->lock();
		}
		~Lock() {
			obj.mtx->unlock();
		}
		operator boolean () const { return locked; }
		void unlock() { locked=false; }
	};
};

template<class T>
class Array : extends Object {
private:
	T *a;
public:
	Array(const Array& o) : length(o.length) {
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
	}
	Array<T>& operator=(const Array<T>&o) {
		if (this == &o) return *this;
		printf("Array copy length=%d\n",o.length);
		delete [] a;
		const_cast<int&>(length) = o.length;
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
		return *this;
	}

	Array<T>& operator=(Array<T>&& o) {
		printf("%s move lenght=%d\n",this->getClass().getName().intern().c_str(),o.length);
		const_cast<int&>(length) = o.length;
		const_cast<int&>(o.length) = 0;
		a = o.a; o.a=null; //a = std::move(o.a); is wrong here
		printf("%s move done\n",this->getClass().getName().intern().c_str());
		return *this;
	}

	const int length;
	Array() : length(0) { a = null; }
	Array(int l) : length(l) { a = new T[l]; }
	~Array() { delete [] a; }
	T& operator[](int i) { return a[i]; }
	const T& operator[](int i) const { return a[i]; }
};

class Integer : extends Object {
public:
	static String toHexString(long v);
};

//usage if: (instanceOf<Integer>(objPtr)) {...}
template<class Base, class T>
inline bool instanceOf(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != null;
}
template<class Base, class T>
inline bool instanceOf(const T& ptr) {
	return dynamic_cast<const Base*>(&ptr) != null;
}

} //namespace lang

#define synchronized(m) for(Object::Lock lock(m); lock; lock.unlock())

using namespace lang;

#endif
