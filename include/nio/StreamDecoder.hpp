#ifndef __NIO_STREAMDECODER_HPP
#define __NIO_STREAMDECODER_HPP

#include <io/InputStream.hpp>
#include <io/Reader.hpp>
#include <nio/charset/Charset.hpp>

namespace nio {

using namespace charset;
using namespace io;

class StreamDecoder : extends Reader {
private:
	static const int DEFAULT_BYTE_BUFFER_SIZE = 8192;
	volatile boolean isOpened = true;
	InputStream *in;
	const Charset& cs;
	int leftoverChar;

	StreamDecoder(InputStream& in, Object* lock, const Charset& cs) : Reader(lock),
		in(&in), cs(cs) {
	}
	const String& encodingName() const {
		return cs.name();
	}
	boolean isOpen() const { return isOpened; }

public:
	static StreamDecoder forInputStreamReader(InputStream& in, Object* lock, const String& charsetName);
	static StreamDecoder forInputStreamReader(InputStream& in, Object* lock, const Charset& cs);
	static StreamDecoder forInputStreamReader(InputStream& in, Object* lock, const CharsetEncoder& enc);

	StreamDecoder(StreamDecoder&& se);
	const String& getEncoding() const {
		if (isOpen()) return encodingName();
		return (const String&)null_obj;
	}

	using Reader::read;
	int read(Array<char>& cbuf, int off, int len) {
		int r = 0;
		synchronized (*lock) {
			r = (int)in->read(&cbuf[0], off, len);
		}
		return r;
	}
	void close() {
		if (in != null) in->close();
	}
};

} //namespace nio

#endif
