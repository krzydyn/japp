#ifndef __UTIL_ARRAYS_HPP
#define __UTIL_ARRAYS_HPP

#include <lang/Math.hpp>
#include <lang/Object.hpp>
#include <lang/System.hpp>

namespace util {

class Arrays final : extends Object {
public:
	template<class T>
	static Array<T> copyOf(Array<T>& original, int newLength) {
		Array<T> copy(newLength);
		System.arraycopy(original, 0, copy, 0, Math.min(original.length, newLength));
		return copy;
	}
};

} //namespace util

#endif
