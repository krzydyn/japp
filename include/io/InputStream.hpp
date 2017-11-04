#ifndef __IO_INPUTSTREAM_HPP
#define __IO_INPUTSTREAM_HPP

#include <io/Closeable.hpp>

namespace io {

class InputStream : public Closeable {
public:
	InputStream& operator=(const InputStream& other) { return *this; }
	InputStream& operator=(InputStream&& other) { return *this; }
};

} //namespace io

#endif
