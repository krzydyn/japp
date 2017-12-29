#ifndef __LANG_READABLE_HPP
#define __LANG_READABLE_HPP

#include <lang/Object.hpp>

namespace nio {
class CharBuffer;
}

namespace lang {

interface Readable : Interface {
public:
	virtual int read(nio::CharBuffer& cb) = 0;
};

} //namespace lang

#endif
