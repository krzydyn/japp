#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <typeinfo>
#include <string>
#include <iostream>

#define boolean bool
#define null nullptr
#define extends public
#define implements virtual public

using jint=long;
using jlong=long long;

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) a ## b
#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)

namespace lang {

class String;
class Object;

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

class Class {
	friend class Object;
private:
	const std::type_info& type;
	Class(const Object& o);
	static std::string nameOf(const std::type_info& type);

public:
	String getName() const;
	String getSimpleName() const;
	String getCanonicalName() const;
	virtual boolean isInstance(const Object& obj) const {return false;}
	virtual boolean isAssignableFrom(const Class& cls) const {return false;}
	virtual boolean isInterface() const {return false;}
	virtual boolean isArray() const {return false;}
	virtual boolean isPrimitive() const {return false;}
	virtual boolean isAnnotation() const {return false;}
	virtual boolean isSynthetic() const {return false;}

	template<class T>
	static std::string nameOf(const T& o) {TRACE;return nameOf(typeid(o)); }
};

class Object {
public:
	virtual ~Object() {}
	virtual const Class getClass() const {return Class(*this);}
	virtual long hashCode() const {return (long)this;}
	virtual long hashCode() {return ((const Object*)this)->hashCode();}
	virtual boolean equals(const Object& obj) const {return this == &obj;}
	virtual Object& clone() const;
	virtual String toString() const;
	virtual void notify() {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final {wait(0); }

	virtual boolean operator==(const Object& obj) final {return equals(obj); }
	virtual boolean operator!=(const Object& obj) final {return !equals(obj); }
	virtual boolean operator==(const void *ptr) {return ptr == this; }
	virtual boolean operator!=(const void *ptr) {return ptr != this; }
protected:
	virtual void finalize() {}
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
