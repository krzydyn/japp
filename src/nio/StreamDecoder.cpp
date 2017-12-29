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

} //namespace nio
