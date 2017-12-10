#ifndef __IO_OUTPUTSTREAMWRITER_HPP
#define __IO_OUTPUTSTREAMWRITER_HPP

#include <lang/Exception.hpp>
#include <nio/StreamEncoder.hpp>

namespace io {

class UnsupportedEncodingException : extends Exception {
};

class OutputStreamWriter : extends Writer {
private:
	nio::StreamEncoder se;

public:
	OutputStreamWriter(OutputStream& out, const String& charsetName) : Writer(&out),
   		se(nio::StreamEncoder::forOutputStreamWriter(out, this, charsetName)) {
	}
	OutputStreamWriter(OutputStream& out) : Writer(&out),
   		se(nio::StreamEncoder::forOutputStreamWriter(out, this, "")) {
	}
};

} //namespace io

#endif
