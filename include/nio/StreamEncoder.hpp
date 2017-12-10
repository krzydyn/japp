#ifndef __NIO_STREAMENCEDER_HPP
#define __NIO_STREAMENCEDER_HPP

#include <io/OutputStream.hpp>
#include <io/Writer.hpp>

namespace nio {

class Charset {
};

class StreamEncoder : extends io::Writer {
private:
	static const int DEFAULT_BYTE_BUFFER_SIZE = 8192;
	volatile boolean isOpen = true;
	io::OutputStream *out;
	Charset cs;
	StreamEncoder(io::OutputStream& out, Object* lock, const Charset& cs) : io::Writer(lock),
		out(&out), cs(cs) {
	}
public:
	static StreamEncoder forOutputStreamWriter(io::OutputStream& out, Object* lock, const String& charsetName);

	StreamEncoder(StreamEncoder&& se);

	using Writer::write;
	void write(const Array<char>& cbuf, int off, int len) {
		synchronized (*lock) {
			//implWrite(cbuf, off, len);
			out->write(&cbuf[0], off, len);
		}
	}
	void flush(){}
	void close(){}
};

} //namespace nio

#endif
