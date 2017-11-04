#ifndef __IO_FLUSHABLE_HPP
#define __IO_FLUSHABLE_HPP

#include <lang/Object.hpp>

namespace io {

class Flushable : Interface {
public:
	virtual void flush() = 0;
};

} //namespace io

#endif
