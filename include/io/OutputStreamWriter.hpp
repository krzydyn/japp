#ifndef __IO_OUTPUTSTREAMWRITER_HPP
#define __IO_OUTPUTSTREAMWRITER_HPP

#include <nio/StreamEncoder.hpp>

namespace io {

using namespace nio::charset;

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
	OutputStreamWriter(OutputStream& out, const Charset& cs) : Writer(&out),
		se(nio::StreamEncoder::forOutputStreamWriter(out, this, cs)) {
	}
	OutputStreamWriter(OutputStream& out, const CharsetEncoder& enc) : Writer(&out),
		se(nio::StreamEncoder::forOutputStreamWriter(out, this, enc)) {
	}
	const String& getEncoding() {
		return se.getEncoding();
	}
	using Writer::write;
	void write(int c) {
		se.write(c);
	}
	void write(const Array<char>& cbuf, int off , int len) {
		se.write(cbuf, off, len);
	}
	void write(const String& str, int off, int len) {
		se.write(str, off, len);
	}
	void flush() {
		se.flush();
	}
	void close() {
		se.close();
	}
};

} //namespace io

#endif
