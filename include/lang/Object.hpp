#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <mutex>
#include <memory> //shared_ptr

#define interface class
#define extends public
#define implements virtual public
#define null nullptr

using boolean=bool;
using byte=unsigned char;
using jint=long;
using jlong=long long;

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) a ## b
#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)
//#define TRACE(a) CallTrace UNIQUE_NAME(the_calltrace)(__FUNCTION__, __FILE__,__LINE__,a);UNIQUE_NAME(the_calltrace).add()

#define class(c) Object::getClass(typeid(c))

namespace lang {

class Class;
class Object;
class String;

extern Object& null_ref;

class Interface {
protected:
	Interface(const Interface& o) = delete;
	Interface(Interface&& o) = delete;
	Interface& operator=(const Interface& o) = delete;
	Interface& operator=(Interface&& o) = delete;
	virtual ~Interface() {}
	Interface() {}
};

#ifndef TRACE
#define TRACE
#undef BACKTRACE
#else
#define BACKTRACE
class Thread;
class CallTrace {
	Thread *t;
public:
	const char *func;
	const char *file;
	unsigned line;
	CallTrace(const char *func, const char *file,unsigned line):func(func),file(file),line(line){t=null;}
	void add();
	~CallTrace();
};
#endif

class Object {
	friend class Lock;
private:
	std::recursive_mutex *mtx=null;
	void move(Object *o) {
		if (this == o) return ;
		mtx = o->mtx; o->mtx = null;
	}
protected:
	virtual void finalize() {}
	virtual Object& clone() const;
public:
	static Class *findClass(const std::type_info& type);
	static void registerClass(Class *c);
	Object(const Object& o) {}
	Object& operator=(const Object& o) {return *this;}
	Object(Object&& o) {move(&o);}
	Object& operator=(Object&& o) {move(&o);return *this;}
	virtual ~Object() { delete mtx; }

	Object() {}
	virtual const Class& getClass() const final;
	virtual jint hashCode() const {return (jint)this;}

	virtual boolean equals(const Object& obj) const {return this == &obj;}
	virtual String toString() const;
	virtual void notify() final {}
	virtual void notifyAll() final {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final {wait(0); }

	boolean operator==(const std::nullptr_t&) const {return this == null;}
	boolean operator!=(const std::nullptr_t&) const {return this != null;}
	boolean operator==(const Object& o) const {return this == &o;}
	boolean operator!=(const Object& o) const {return this != &o;}

	static const Class& getClass(const std::type_info& type);

	class Lock {
	private:
		const Object& obj;
		boolean locked=true;
	public:
		Lock(const Object* o) : Lock(*o) {}
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

template<class T> using Shared = std::shared_ptr<T>;
template<class T, class... Args>
Shared<T> makeShared(Args&&... args) {TRACE; return std::make_shared<T>(args...); }


void registerArrayClass(const std::type_info& type);
void checkArrayBounds(int i, int l);
template<class T>
class Array : extends Object {
private:
	T *a;
	void init() { registerArrayClass(typeid(*this)); }

public:
	const int length;
	Array(const Array<T>& o) : length(o.length) {
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
	}
	Array(Array<T>&& o) : length(o.length) {
		const_cast<int&>(o.length) = 0;
		a = o.a; o.a = null;
	}
	Array<T>& operator=(const Array<T>&o) {
		if (this == &o) return *this;
		delete [] a;
		const_cast<int&>(length) = o.length;
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
		return *this;
	}

	Array<T>& operator=(Array<T>&& o) {
		const_cast<int&>(length) = o.length; const_cast<int&>(o.length) = 0;
		a = o.a; o.a = null;
		return *this;
	}

	Array() : length(0) {init(); a = null; }
	Array(int l) : length(l) {
		checkArrayBounds(l, l+1);
		init(); a = new T[l];
	}
	Array(T* v, int l) : length(l) {
		checkArrayBounds(l, l+1);
		init(); a = new T[l];
		for (int i=0; i < l; ++i) a[i]=v[i];
	}
	~Array() { delete [] a; }
	T& operator[](int i) {
		checkArrayBounds(i, length);
		return a[i];
	}
	const T& operator[](int i) const {
		checkArrayBounds(i, length);
		return a[i];
	}
};

//usage if: (instanceOf<Integer>(objPtr)) {...}
template<class Base, class T>
inline bool instanceOf(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != null;
}

} //namespace lang

#define synchronized(m) for(Object::Lock __lck(m); __lck; __lck.unlock())

using namespace lang;

//template<class T, class std::enable_if<std::is_base_of<Object,T>::value,Object>::type* = nullptr>
//using classT = typename(T)::theclass;

#endif
