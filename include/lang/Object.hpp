#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

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

class CallTrace {
public:
	const char *func;
	const char *file;
	unsigned line;
	CallTrace(const char *func, const char *file,unsigned line):func(func),file(file),line(line){}
	void r();
	~CallTrace();
};

#define TRACE CallTrace UNIQUE_NAME(the_calltrace)(__FUNCTION__, __FILE__,__LINE__);UNIQUE_NAME(the_calltrace).r()

#ifndef TRACE
#define TRACE
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
protected:
	virtual void finalize() {}
	virtual Object& clone() const;
public:
	virtual ~Object() {}
	virtual const Class getClass() const;
	virtual long hashCode() const {return (long)this;}
	virtual long hashCode() {return ((const Object*)this)->hashCode();}
	virtual boolean equals(const Object& obj) const {return this == &obj;}
	virtual String toString() const;
	virtual void notify() {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final {wait(0); }

	virtual boolean operator==(const Object& obj) final {return equals(obj); }
	virtual boolean operator!=(const Object& obj) final {return !equals(obj); }
	virtual boolean operator==(const void *ptr) {return ptr == this; }
	virtual boolean operator!=(const void *ptr) {return ptr != this; }
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

using namespace lang;

#endif
