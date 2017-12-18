#include <nio/StreamEncoder.hpp>
#include <io/IOException.hpp>

namespace nio {

StreamEncoder StreamEncoder::forOutputStreamWriter(OutputStream& out, Object* lock, const String& charsetName) {
	String csn = charsetName;
	if (csn == null_ref) {
		csn = Charset::defaultCharset().name();
	}
	try {
		if (Charset::isSupported(csn))
			return StreamEncoder(out, lock, Charset::forName(csn));
	//} catch (IllegalCharsetNameException x) { }
	} catch (const Exception& x) { }
	throw UnsupportedEncodingException(csn);
}

} //namespace io
