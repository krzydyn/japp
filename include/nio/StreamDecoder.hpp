#ifndef __NIO_STREAMDECODER_HPP
#define __NIO_STREAMDECODER_HPP

#include <lang/System.hpp>
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
	int leftoverChar = -1;
	const Charset& cs;
	//CharsetDecoder decoder;
	//ByteBuffer bb;

	InputStream *in;
	//ReadableByteChannel *ch;

	StreamDecoder(InputStream& in, Object* lock, const Charset& cs) : Reader(lock),
		cs(cs), in(&in) {
		System.out.println("StreamDecoder created "+in.toString());
	}
	const String& encodingName() const {
		return cs.name();
	}
	boolean isOpen() const { return isOpened; }
	void ensureOpen() {
		if (!isOpened) throw IOException("Stream closed");
	}

	boolean inReady() {
		return in->available() > 0;
	}
	boolean implReady() {
		return inReady();
	}
	int implRead(Array<char>& cbuf, int off, int end) {
		System.out.printf("off = %d, end=%d\n",off,end);
		//TODO Shared<CharBuffer> cb = CharBuffer::wrap(cbuf, off, end - off);
		return in->read(&cbuf[0], off, end - off);
	}
	void implClose() {
		if (in != null) in->close();
	}

	int read0() {
		int r = leftoverChar;
		if (r >= 0) {
			leftoverChar = -1;
			return r;
		}
		Array<char> cb(2);
		int n = read(cb, 0, 2);
		switch (n) {
			case 2:
				leftoverChar = cb[1]&0xffff;
				// FALL THROUGH
			case 1:
				return cb[0];
			default:
				break;
		}
		return -1;
	}

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
	int read() {
		synchronized (*lock) {
			return read0();
		}
		return -1;
	}
	int read(Array<char>& cbuf, int off, int len) {
		if ((off < 0) || (off > cbuf.length) || (len < 0) ||
				((off + len) > cbuf.length) || ((off + len) < 0)) {
			throw IndexOutOfBoundsException();
		}
		if (len == 0) return 0;
		if (off >= cbuf.length) throw BufferOverflowException();
		int n = 0;
		synchronized (*lock) {
			if (leftoverChar >= 0) {
				cbuf[off] = (char)leftoverChar;
				off++; len--; n = 1;
				leftoverChar = -1;
			}
			if (len == 0) return n;
			ensureOpen();
			if (len == 1) {
				int c = read0();
				if (c == -1) return (n == 0) ? -1 : n;
				cbuf[off] = (char)c;
				return n + 1;
			}
			return n + implRead(cbuf, off, off + len);
		}
		return -1;
	}

	boolean ready() {
		synchronized (*lock) {
			ensureOpen();
			return leftoverChar >= 0 || implReady();
		}
		return false;
	}
	long skip(long n) {
		return in->skip(n);
	}
	void close() {
		synchronized (lock) {
			if (!isOpened) return ;
			implClose();
			isOpened = false;
		}
	}
};

} //namespace nio

#endif
