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
	String toGenericString() const {
	}
	String getName() const;
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
	static String nameOf(const T& o) {TRACE;return getTypeName(typeid(o)); }
};

} //namespace lang

#endif
