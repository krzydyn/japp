#ifndef __LANG_CLASS_HPP
#define __LANG_CLASS_HPP

#include <lang/Object.hpp>
#include <typeinfo>

namespace lang {

class Class : extends Object {
	friend class Object;
private:
	const std::type_info& type;
protected:
	Class(const std::type_info& t) : type(t) {}
public:
	String toString() const;
	/*
	 * Returns a string describing this {@code Class}, including
	 * information about modifiers and type parameters.
	 * @since 1.8
	 */
	String toGenericString() const;
	String getName() const;
	String getSimpleName() const;
	String getCanonicalName() const;
	boolean isInstance(const Object& obj) const {
		//return instanceof<typename(obj)>(this);
		return false;
	}
	virtual boolean isAssignableFrom(const Class& cls) const {return false;}
	virtual boolean isInterface() const {return false;}
	virtual boolean isArray() const {return false;}
	virtual boolean isPrimitive() const {return false;}
	virtual boolean isAnnotation() const {return false;}
	virtual boolean isSynthetic() const {return false;}

	boolean equals(const Class& o) const {
		return type == o.type;
	}
};

} //namespace lang

#endif
