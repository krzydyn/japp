#ifndef __NIO_CHARSET_HPP
#define __NIO_CHARSET_HPP

#include <lang/Comparable.hpp>
#include <lang/Exception.hpp>
#include <lang/String.hpp>

namespace nio {
namespace charset {

class UnsupportedCharsetException : extends IllegalArgumentException {
	using IllegalArgumentException::IllegalArgumentException;
};

class IllegalCharsetNameException : extends IllegalArgumentException {
	using IllegalArgumentException::IllegalArgumentException;
};


class Charset;
class CharsetEncoder;
class CharsetDecoder;
class CharsetProvider : extends Object {
protected:
	CharsetProvider() {}
public:
	virtual SharedIterator<Charset> charsets() const = 0;
	virtual const Charset& charsetForName(const String& charsetName) const = 0;
};

class Charset : extends Object, implements Comparable<Charset> {
private:
	static const CharsetProvider& standardProvider;
	static const Charset& defCharset;

	static void checkName(const String& s) {
	}
	static const Charset& lookup(const String& charsetName);

	String pName;
	Array<String> pAliases;

	void copy(const Charset& o) {
		pName = o.pName;
		pAliases = o.pAliases;
	}
	void move(const Charset& o) {
		pName = std::move(o.pName);
		pAliases = std::move(o.pAliases);
	}

protected:
	Charset(const String& canonicalName, const Array<String>& aliases) : pName(canonicalName), pAliases(aliases) {
		for (int i = 0; i < this->pAliases.length; i++)
			checkName(this->pAliases[i]);
	}

public:
	static boolean isSupported(const String& charsetName) {TRACE;
		return (Charset::lookup(charsetName) != null);
	}
	static const Charset& forName(const String& charsetName) {TRACE;
		const Charset& cs = Charset::lookup(charsetName);
		if (cs == null) throw UnsupportedCharsetException(charsetName);
		return cs;
	}
	static const Charset& defaultCharset() {TRACE;
		return defCharset;
	}

	Charset(){}
	Charset(const Charset& o) : pName(o.pName), pAliases(o.pAliases) {TRACE;
	}
	Charset& operator=(const Charset& o) { copy(o); return *this; }
	Charset& operator=(Charset&& o) { move(o); return *this; }

	virtual const String& name() const final {TRACE;
		return pName;
	}
	virtual const Array<String>& aliases() const final {TRACE;
		return pAliases;
	}
	virtual String displayName() const { return pName; }
	virtual boolean isRegistered() const final { return !pName.startsWith("X-") && !pName.startsWith("x-"); }
	virtual boolean contains(const Charset& cs) const = 0;
	virtual Shared<CharsetDecoder> newDecoder() const = 0;
	virtual Shared<CharsetEncoder> newEncoder() const = 0;
	virtual boolean canEncode() const { return true; }
	//public final CharBuffer decode(ByteBuffer bb)
	//public final ByteBuffer encode(CharBuffer cb)
	//public final ByteBuffer encode(String str)
	virtual int compareTo(const Charset& that) const final {TRACE;
		return pName.compareToIgnoreCase(that.pName);
	}
	int hashCode() const final { return pName.hashCode(); }
	boolean equals(const Object& o) const {
		if (!instanceof<Charset>(&o)) return false;
		if (this == &o) return true;
		return pName.equals(((const Charset&)o).name());
	}
	String toString() const final { return pName; }
};

class CharsetEncoder : extends Object {
private:
	static const int ST_RESET   = 0;
	static const int ST_CODING  = 1;
	static const int ST_END     = 2;
	static const int ST_FLUSHED = 3;
	static Array<String> stateNames; // = { "RESET", "CODING", "CODING_END", "FLUSHED" };

	Charset& chset;
	float averageBytesPerChar;
	float maxBytesPerChar;
	Array<byte> replacement;
	int state = ST_RESET;

protected:
	CharsetEncoder(Charset& cs, float averageBytesPerChar, float maxBytesPerChar, const Array<byte>& replacement) :
   			chset(cs), averageBytesPerChar(averageBytesPerChar), maxBytesPerChar(maxBytesPerChar)	{
		replaceWith(replacement);
	}TRACE;
	CharsetEncoder(Charset& cs, float averageBytesPerChar, float maxBytesPerChar) :
			CharsetEncoder(cs, averageBytesPerChar, maxBytesPerChar, Array<byte>((byte)'?')) {TRACE;
	}
public:
	const Charset& charset() const { return chset; }

	CharsetEncoder& replaceWith(const Array<byte>& newReplacement) {
		return *this;
	}
};

}} //namespace io::charset

#endif
