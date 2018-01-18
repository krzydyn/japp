#include <nio/StreamDecoder.hpp>
#include <io/IOException.hpp>

namespace nio {

StreamDecoder StreamDecoder::forInputStreamReader(InputStream& in, Object* lock, const String& charsetName) {
	String csn;
	if (charsetName == null_obj) {
		if (Charset::defaultCharset() == null_obj) throw UnsupportedEncodingException(null_obj.toString());
		csn = Charset::defaultCharset().name();
	}
	else csn = charsetName;
	try {
		if (Charset::isSupported(csn))
			return StreamDecoder(in, lock, Charset::forName(csn));
	//} catch (IllegalCharsetNameException x) { }
	} catch (const Exception& x) { }
	throw UnsupportedEncodingException(csn);
}

int StreamDecoder::readBytes() {
	bb->compact();
	int lim = bb->limit();
	int pos = bb->position();
	int rem = (pos <= lim ? lim - pos : 0);
	if (rem == 0) {
		throw RuntimeException("No place to read bb=" + bb->toString());
	}
	int n = in->read(bb->array(), bb->arrayOffset() + pos, rem);
	if (n < 0) return n;
	if (n == 0) throw IOException("Underlying input stream returned zero bytes");
	bb->position(pos + n);
	bb->flip();
	return bb->remaining();
}

int StreamDecoder::implRead(Array<char>& cbuf, int off, int end) {
	Log.log("StreamDecoder::implRead: cbuf.len=%d off = %d, end=%d",cbuf.length,off,end);
	Shared<CharBuffer> cb = CharBuffer::wrap(cbuf, off, end - off);
	// Ensure that cb[0] == cbuf[off]
	if (cb->position() != 0) cb = cb->slice();

	boolean eof = false;
	for (;;) {
		nio::CoderResult cr = decoder->decode(*bb, *cb, eof);
		if (cr.isUnderflow()) {
			if (eof) break;
			if (!cb->hasRemaining()) break;
			if (cb->position() > 0 && !inReady()) break;
			int n = readBytes();
			if (n < 0) {
				eof = true;
				if (cb->position() == 0 && !bb->hasRemaining()) break;
				decoder->reset();
			}
			continue;
		}
		if (cr.isOverflow()) {
			break;
		}
		cr.throwException();
	}
	if (eof) decoder->reset();

	if (cb->position() == 0) {
		if (eof) return -1;
	}
	return cb->position();
}

} //namespace nio
