#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <typeinfo>
#include <string>

#define boolean bool
#define null nullptr
#define extends public
#define implements virtual public

using jint=long;
using jlong=long long;

namespace lang {

class String;
class Object;

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
	static std::string nameOf(const T& o) { return nameOf(typeid(o)); }
};

class Object {
public:
	virtual ~Object() {}
	virtual const Class getClass() const { return Class(*this); }
	virtual long hashCode() const {return (long)this;}
	virtual long hashCode() {return ((const Object*)this)->hashCode();}
	virtual boolean equals(const Object& obj) const { return this == &obj; }
	virtual Object& clone() const;
	virtual String toString() const;
	virtual void notify() {}
	virtual void wait(long timeout) final {}
	virtual void wait(long timeout, int nanos) final {}
	virtual void wait() final { wait(0); }

	virtual boolean operator==(const Object& obj) final { return equals(obj); }
	virtual boolean operator!=(const Object& obj) final { return !equals(obj); }
	virtual boolean operator==(const void *ptr) { return ptr == this; }
	virtual boolean operator!=(const void *ptr) { return ptr != this; }
protected:
	virtual void finalize() {}
};

template<class T>
class Array : extends Object {
private:
	T *a;
public:
	const int length;
	Array(int l) : length(l) { a = new T[l]; }
	~Array() { if (a) delete a; }
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
	return dynamic_cast<const Base*>(ptr) != nullptr;
}
template<class Base, class T>
inline bool instanceOf(const T& ptr) {
	return dynamic_cast<const Base*>(&ptr) != nullptr;
}

}
using namespace lang;

#endif
