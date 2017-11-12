#ifndef __UTIL_ITERATOR_HPP
#define __UTIL_ITERATOR_HPP

#include <lang/Exception.hpp>
#include <memory>

namespace util {

template<class T>
interface Iterator : Interface {
protected:
	Iterator() {}
public:
	virtual bool hasNext() const = 0;
	virtual const T& next() = 0;
	virtual void remove() {
		throw UnsupportedOperationException("remove");
	}
};

template<class T>
using IteratorPtr = std::shared_ptr<Iterator<T>>;

template<class T, class... Args>
std::shared_ptr<T> makeIterator(Args&&... args) { return std::make_shared<T>(args...); }

} //namespace util

#endif
