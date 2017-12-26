#include <nio/StreamEncoder.hpp>
#include <io/IOException.hpp>

namespace nio {

StreamEncoder StreamEncoder::forOutputStreamWriter(OutputStream& out, Object* lock, const String& charsetName) {
	String csn;
	if (charsetName == null_obj) {
		if (Charset::defaultCharset() == null_obj) throw UnsupportedEncodingException(null_obj.toString());
		csn = Charset::defaultCharset().name();
	}
	else csn = charsetName;
	try {
		if (Charset::isSupported(csn))
			return StreamEncoder(out, lock, Charset::forName(csn));
	//} catch (IllegalCharsetNameException x) { }
	} catch (const Exception& x) { }
	throw UnsupportedEncodingException(csn);
}

} //namespace io
