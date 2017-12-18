#ifndef __NIO_CHARSET_HPP
#define __NIO_CHARSET_HPP

#include <lang/Comparable.hpp>
#include <lang/Exception.hpp>
#include <lang/String.hpp>

namespace nio {
namespace charset {

class UnsupportedCharsetException : extends IllegalArgumentException {
public:
	UnsupportedCharsetException() : IllegalArgumentException() {}
	UnsupportedCharsetException(const String& msg) : IllegalArgumentException(msg) {}
};

class IllegalCharsetNameException : extends IllegalArgumentException {
};

class Charset;
class CharsetProvider : extends Object {
protected:
	CharsetProvider() {}
public:
	virtual SharedIterator<Charset> charsets() = 0;
	virtual Charset* charsetForName(const String& charsetName) = 0;
};

class Charset : extends Object, implements Comparable<Charset> {
private:
	static CharsetProvider& standardProvider;
	static Charset defCharset;

	static void checkName(const String& s) {
	}
	static Charset* lookup(const String& charsetName);

	const String canonName;
	const Array<String>& aliases;

protected:
	Charset(const String& canonicalName, const Array<String>& aliases) : canonName(canonicalName), aliases(aliases) {
		for (int i = 0; i < this->aliases.length; i++)
			checkName(this->aliases[i]);
	}

public:
	static boolean isSupported(const String& charsetName) {TRACE;
		return (lookup(charsetName) != null);
	}
	static const Charset& forName(const String& charsetName) {TRACE;
		Charset* cs = lookup(charsetName);
		if (cs == null) throw UnsupportedCharsetException(charsetName);
		return *cs;
	}
	static const Charset& defaultCharset() {TRACE;
		return defCharset;
	}

	Charset(const Charset& cs) : canonName(cs.canonName), aliases(cs.aliases) {
	}
	const String& name() const {
		return canonName;
	}

	int compareTo(const Charset& that) const final {
		return name().compareToIgnoreCase(that.name());
	}
};

class CharsetEncoder {
private:
	static const int ST_RESET   = 0;
	static const int ST_CODING  = 1;
	static const int ST_END     = 2;
	static const int ST_FLUSHED = 3;
	static Array<String> stateNames; // = { "RESET", "CODING", "CODING_END", "FLUSHED" };

	Charset chset;
	float averageBytesPerChar;
	float maxBytesPerChar;
	Array<byte> replacement;
	int state = ST_RESET;

protected:
	CharsetEncoder(const Charset& cs, float averageBytesPerChar, float maxBytesPerChar, const Array<byte>& replacement) :
   			chset(cs), averageBytesPerChar(averageBytesPerChar), maxBytesPerChar(maxBytesPerChar)	{
		replaceWith(replacement);
	}
	CharsetEncoder(Charset cs, float averageBytesPerChar, float maxBytesPerChar) :
			CharsetEncoder(cs, averageBytesPerChar, maxBytesPerChar, Array<byte>((byte)'?')) {
	}
public:
	const Charset& charset() const { return chset; }

	CharsetEncoder& replaceWith(const Array<byte>& newReplacement) {
		return *this;
	}
};

}} //namespace io::charset

#endif
