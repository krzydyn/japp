#ifndef __IO_CLOSABLE_HPP
#define __IO_CLOSABLE_HPP

namespace io {

class Closeable {
public:
	virtual void close() = 0;
};

} //namespace io

#endif
