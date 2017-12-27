#ifndef __IO_READABLE_HPP
#define __IO_READABLE_HPP

namespace nio {
class CharBuffer;
}

namespace io {

interface Readable : Interface {
public:
	virtual int read(nio::CharBuffer& cb) = 0;
};

} //namespace io

#endif
