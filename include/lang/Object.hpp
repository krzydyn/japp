#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <typeinfo>

#define boolean bool
#define null 0
#define eol ((unsigned)-1)

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
	boolean isInstance(const Object& obj) const {}
	boolean isAssignableFrom(const Class& cls) const {}
	boolean isInterface() const {}
	boolean isArray() const {}
	boolean isPrimitive() const {}
	boolean isAnnotation() const {}
	boolean isSynthetic() const {}
	String getSimpleName() const;
	String getCanonicalName() const;
};

class Object {
public:
	virtual ~Object() {}
	Class getClass() const { return Class(typeid(*this)); }
	virtual long hashCode() const {return (long)this;}
	virtual boolean equals(const Object& obj) { return this == &obj; }
	virtual Object& clone() {}
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

}
using namespace lang;

template<typename Base, typename T>
inline bool instanceOf(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}
//usage if: (instanceOf<Base>(objPtr)) {...}
template<typename Base, typename T>
inline bool instanceOf(const T& ptr) {
	return dynamic_cast<const Base*>(&ptr) != nullptr;
}

#endif
