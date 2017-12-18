#ifndef __LANG_COMPARABLE_HPP
#define __LANG_COMPARABLE_HPP

#include <lang/Object.hpp>

namespace lang {

template<class T>
interface Comparable : Interface {
public:
	virtual int compareTo(const T& o) const = 0;
};

} //namespace lang

#endif
