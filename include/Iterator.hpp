#ifndef __ITERATOR_HPP
#define __ITERATOR_HPP

#include <memory>
#include <lang/Exceptions.hpp>

template<class T>
class Iterator {
public:
	virtual ~Iterator() {}
	virtual bool hasNext() = 0;
	virtual const T& next() = 0;
	virtual void remove() {
		throw lang::UnsupportedOperationException("remove");
	}
};

#endif
