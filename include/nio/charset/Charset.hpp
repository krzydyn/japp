#ifndef __NIO_CHARSET_HPP
#define __NIO_CHARSET_HPP

namespace nio {
namespace charset {

class UnsupportedCharsetException : extends IllegalArgumentException {
public:
	UnsupportedCharsetException() : IllegalArgumentException() {}
	UnsupportedCharsetException(const String& msg) : IllegalArgumentException(msg) {}
};

class Charset {
private:
	static Charset defCharset;

	static void checkName(const String& s) {
	}

	const String canonName;
	const Array<String>& aliases;

	static Charset* lookup(const String& charsetName) {
		return null;
	}

protected:
	Charset(const String& canonicalName, const Array<String>& aliases) : canonName(canonicalName), aliases(aliases) {
		for (int i = 0; i < this->aliases.length; i++)
			checkName(this->aliases[i]);
	}

public:
	static const Charset& forName(const String& charsetName) {
		Charset* cs = lookup(charsetName);
		if (cs != null) throw UnsupportedCharsetException(charsetName);
		return *cs;
	}
	static const Charset& defaultCharset() {
		return defCharset;
	}

	const String& name() {
		return canonName;
	}
};

class CharsetEncoder {
};

}} //namespace io::charset

#endif
