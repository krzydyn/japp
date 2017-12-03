#ifndef __IO_FLUSHABLE_HPP
#define __IO_FLUSHABLE_HPP

#include <lang/Object.hpp>

namespace io {

interface Flushable : Interface {
public:
	virtual void flush() = 0;
};

} //namespace io

#endif
