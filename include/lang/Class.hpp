#ifndef __LANG_CLASS_HPP
#define __LANG_CLASS_HPP

#include <lang/String.hpp>
#include <typeinfo>

namespace lang {

class Class : extends Object {
	friend class Object;
private:
	const std::type_info& type;
	Class(const Object& o);

	static String getTypeName(const std::type_info&);
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
	static String nameOf(const T& o) {TRACE;return getTypeName(typeid(o)); }
};

} //namespace lang

#endif
