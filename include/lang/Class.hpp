#ifndef __LANG_CLASS_HPP
#define __LANG_CLASS_HPP

#include <lang/String.hpp>
#include <typeinfo>

namespace lang {

class Class final : extends Object {
	friend class Object;
private:
	const std::type_info& type;
	Class(const Object& o);

	static String getTypeName(const std::type_info&);
public:
	String toString() const {
		String c = isInterface() ? "interface " : (isPrimitive() ? "" : "class ");
		return c + getName();
	}
	/*
	 * Returns a string describing this {@code Class}, including
	 * information about modifiers and type parameters.
	 * @since 1.8
	 */
	String toGenericString() const {return toString();}
	String getName() const {return getTypeName(type);}
	String getSimpleName() const;
	String getCanonicalName() const;
	boolean isInstance(const Object& obj) const {return false;}
	boolean isAssignableFrom(const Class& cls) const {return false;}
	boolean isInterface() const {return false;}
	boolean isArray() const {return false;}
	boolean isPrimitive() const {return false;}
	boolean isAnnotation() const {return false;}
	boolean isSynthetic() const {return false;}

	template<class T>
	static String typeOf(const T& o) {return getTypeName(typeid(o)); }
};

} //namespace lang

#endif
