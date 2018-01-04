#include <io/Reader.hpp>
#include <nio/StreamDecoder.hpp>

namespace io {

class InputStreamReader : extends Reader {
private:
	nio::StreamDecoder sd;

public:
	InputStreamReader(InputStream& in, const String& charsetName) : Reader(&in),
		sd(nio::StreamDecoder::forInputStreamReader(in, this, charsetName)) {
	}
	InputStreamReader(InputStream& in) : Reader(&in),
		sd(nio::StreamDecoder::forInputStreamReader(in, this, (String&)null_obj)) {
	}

	const String& getEncoding() const {
		return sd.getEncoding();
	}

	using Reader::read;
	int read() { return sd.read(); }
	int read(Array<char>& cbuf, int off, int len) {
		return sd.read(cbuf, off, len);
	}
	boolean ready() {
		return sd.ready();
	}

	void close() {
		sd.close();
	}
};

}
