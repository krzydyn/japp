#ifndef __LANG_OBJECT_HPP
#define __LANG_OBJECT_HPP

#include <typeinfo>

#define boolean bool
#define null 0

using jint=long;
using jlong=long long;

namespace lang {

template<class T>
class Array {
private:
	T *a;
public:
	const int length;
	Array(int l) : length(l) { a = new T[l]; }
	~Array() { delete a; }
	T& operator[](int i) {
		return a[i];
	}
	const T& operator[](int i) const { 
		return a[i];
	}
};

class String;
class Object;

class ObjectInfo {
	
};
class Class {
friend class Object;
private:
	const std::type_info& type;
	Class(const std::type_info& t, const ObjectInfo&) : type(t) {}
public:

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
	virtual const Class getClass() const;
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

class Integer : public Object {
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
