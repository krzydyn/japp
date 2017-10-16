#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <typeinfo>

#define boolean bool
#define null 0

using jint=long;
using jlong=long long;

namespace lang {

class String;
class Object;

class Class {
private:
	const std::type_info& type;
public:
	Class(const std::type_info& t) : type(t) {}

	String getName() const;
	boolean isInstance(const Object& obj) const {return false;}
	boolean isAssignableFrom(const Class& cls) const {return false;}
	boolean isInterface() const {return false;}
	boolean isArray() const {return false;}
	boolean isPrimitive() const {return false;}
	boolean isAnnotation() const {return false;}
	boolean isSynthetic() const {return false;}
	String getSimpleName() const;
	String getCanonicalName() const;
};

class Object {
public:
	virtual ~Object() {}
	Class getClass() const { return Class(typeid(*this)); }
	virtual long hashCode() const {return (long)this;}
	virtual boolean equals(const Object& obj) const { return this == &obj; }
	virtual Object& clone() {return *this;}
	virtual String toString() const;
	virtual void notify() {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final { wait(0); }

protected:
	virtual void finalize() {}
};

class Integer : public Object {
public:
	static String toHexString(int v);
};

//usage if: (instanceOf<Integer>(objPtr)) {...}
template<class Base, class T>
inline bool instanceOf(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}
template<class Base, class T>
inline bool instanceOf(const T& ptr) {
	return dynamic_cast<const Base*>(&ptr) != nullptr;
}

}
using namespace lang;

#endif
