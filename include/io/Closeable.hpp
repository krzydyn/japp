#ifndef __IO_CLOSABLE_HPP
#define __IO_CLOSABLE_HPP

#include <lang/Object.hpp>

namespace io {

interface Closeable : Interface {
public:
	virtual void close() = 0;
};

} //namespace io

#endif
