#ifndef __NIO_STREAMENCEDER_HPP
#define __NIO_STREAMENCEDER_HPP

#include <io/OutputStream.hpp>
#include <io/Writer.hpp>
#include <nio/charset/Charset.hpp>

namespace nio {

using namespace charset;
using namespace io;

class StreamEncoder : extends Writer {
private:
	static const int DEFAULT_BYTE_BUFFER_SIZE = 8192;
	volatile boolean isOpened = true;
	OutputStream *out;
	const Charset& cs;
	StreamEncoder(OutputStream& out, Object* lock, const Charset& cs) : Writer(lock),
		out(&out), cs(cs) {
	}
	const String& encodingName() const {
		return cs.name();
	}
	boolean isOpen() const { return isOpened; }

public:
	static StreamEncoder forOutputStreamWriter(OutputStream& out, Object* lock, const String& charsetName);
	static StreamEncoder forOutputStreamWriter(OutputStream& out, Object* lock, const Charset& cs);
	static StreamEncoder forOutputStreamWriter(OutputStream& out, Object* lock, const CharsetEncoder& enc);

	StreamEncoder(StreamEncoder&& se);
	const String& getEncoding() const {
		if (isOpen()) return encodingName();
		return (const String&)null_obj;
	}

	using Writer::write;
	void write(const Array<char>& cbuf, int off, int len) {
		synchronized (*lock) {
			//implWrite(cbuf, off, len);
			out->write(&cbuf[0], off, len);
		}
	}
	void flush() {
		if (out != null) out->flush();
	}
	void close() {
		if (out != null) out->close();
	}

	void flushBuffer() {
	}
};

} //namespace nio

#endif
