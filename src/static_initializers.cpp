#include <lang/Math.hpp>
#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileInputStream.hpp>
#include <io/FileOutputStream.hpp>
#include <nio/ByteBuffer.hpp>
#include <nio/CharBuffer.hpp>
#include <nio/charset/CharsetDecoder.hpp>
#include <nio/channels/SelectorProvider.hpp>
#include <chrono>
#include <cstdlib>

namespace {
io::InputStream& std_in() {
	static io::FileInputStream stream(std::cin);
	return stream;
}
io::PrintStream& std_out() {
	static io::FileOutputStream fstream(std::cout);
	static io::PrintStream stream(fstream);
	//std::cerr << "std_out init" << std::endl;
	return stream;
}
io::PrintStream& std_err() {
	static io::FileOutputStream fstream(std::cerr);
	static io::PrintStream stream(fstream);
	//std::cerr << "std_err init" << std::endl;
	return stream;
}

HashMap<String,String>& get_env() {
	static HashMap<String,String> env;
	return env;
}

using namespace nio;
using namespace nio::charset;
// http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/sun/nio/cs/US_ASCII.java#US_ASCII.Decoder
class US_ASCII_Decoder : extends CharsetDecoder {
public:
	US_ASCII_Decoder(const Charset *cs) : CharsetDecoder(cs, 1.0f, 1.0f) {
	}

private:
	CoderResult decodeArrayLoop(ByteBuffer& src, CharBuffer& dst) {
		LOGD("US_ASCII_Decoder::decodeArrayLoop");
		Array<byte>& sa = src.array();
		int sp = src.arrayOffset() + src.position();
		int sl = src.arrayOffset() + src.limit();
		sp = (sp <= sl ? sp : sl);
		Array<char>& da = dst.array();
		int dp = dst.arrayOffset() + dst.position();
		int dl = dst.arrayOffset() + dst.limit();
		dp = (dp <= dl ? dp : dl);
		Finalize(
			src.position(sp - src.arrayOffset());
			dst.position(dp - dst.arrayOffset());
		);
		while (sp < sl) {
			byte b = sa[sp];
			if (b >= 0) {
				if (dp >= dl) return CoderResult::OVERFLOW;
				da[dp++] = (char)b;
				sp++;
				continue;
			}
			return CoderResult::malformedForLength(1);
		}
		return CoderResult::UNDERFLOW;
	}
	CoderResult decodeBufferLoop(ByteBuffer& src, CharBuffer& dst) {
		int mark = src.position();
		LOGD("US_ASCII_Decoder::decodeBufferLoop mark=%d", mark);
		Finalize(src.position(mark););
		while (src.hasRemaining()) {
			LOGD("src.hasRem=%d, dst.hasRem=%d", src.remaining(), dst.remaining());
			byte b = src.get();
			if (b >= 0) {
				if (!dst.hasRemaining()) return CoderResult::OVERFLOW;
				dst.put((char)b);
				mark++;
				continue;
			}
			return CoderResult::malformedForLength(1);
		}
		return CoderResult::UNDERFLOW;
	}

protected:
	CoderResult decodeLoop(ByteBuffer& src, CharBuffer& dst) {
		if (src.hasArray() && dst.hasArray())
			return decodeArrayLoop(src, dst);
		else
			return decodeBufferLoop(src, dst);
	}
};

class US_ASCII : extends Charset {
public:
	US_ASCII() : Charset("US-ASCII",Array<String>()) {}
	String historicalName() { return "ASCII"; }
	boolean contains(const Charset& cs) const {
		return true;
	}
	Shared<CharsetDecoder> newDecoder() const {
		return makeShared<US_ASCII_Decoder>(this);
	}
	Shared<CharsetEncoder> newEncoder() const {
		return null;
	}
};

// http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/sun/nio/cs/UTF_8.java#UTF_8.Decoder
class UTF8_Decoder : extends CharsetDecoder {
public:
	UTF8_Decoder(const Charset *cs) : CharsetDecoder(cs, 1.0f, 1.0f) {
	}

protected:
	CoderResult decodeLoop(ByteBuffer& src, CharBuffer& dst) {
		LOGD("UTF8_Decoder::decodeLoop");
		int mark = src.position();
		int limit = src.limit();
		while (mark < limit && dst.remaining() > 0) {
			int b1 = src.get();
			dst.put((char)b1);
		}
		CoderResult cr = src.remaining() > 0 && dst.remaining() == 0 ?
				CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
		src.position(mark);
		return cr;
	}
};
class UTF_8 : extends Charset {
public:
	UTF_8() : Charset("UTF-8",Array<String>()) {}
	String historicalName() { return "UTF-8"; }
	boolean contains(const Charset& cs) const {
		return true;
	}
	Shared<CharsetDecoder> newDecoder() const {
		return makeShared<UTF8_Decoder>(this);
	}
	Shared<CharsetEncoder> newEncoder() const {
		return null;
	}
};

class FastCharsetProvider : extends CharsetProvider {
private:
	static String toLower(const String& s) {
		int n = s.length();
		boolean allLower = true;
		for (int i = 0; i < n; i++) {
			int c = s.charAt(i);
			if (((c - 'A') | ('Z' - c)) >= 0) {
				allLower = false;
				break;
			}
		}
		if (allLower) return s;
		//TODO don't use String::toLowerCase, it's based on Charset
		return s.toLowerCase();
	}

	HashMap<String,String> aliasMap;
	HashMap<String,Charset*> cache;

	const String& canonicalize(const String& csn) const {
		const String& acn = aliasMap.get(csn);
		return (acn != null_obj) ? acn : csn;
	}
	const Charset& lookup(const String& charsetName) const {TRACE;
		const String csn = canonicalize(toLower(charsetName));

		const Charset* cs = cache.get(csn);
		if (cs != null) return *cs;

		System.out.println("fast::lookup(" + charsetName + ") = null");
		return (Charset&)null_obj;
	}
	void registerCharset(Charset& cs) {
		String csn = canonicalize(toLower(cs.name()));
		cache.put(csn, &cs);
	}
	US_ASCII usascii;
	UTF_8 utf8;
public:
	FastCharsetProvider() {
		registerCharset(usascii);
		registerCharset(utf8);
	}
	SharedIterator<Charset> charsets() const {
		return null;
	}
	const Charset& charsetForName(const String& charsetName) const {
		return lookup(charsetName);
	}
};

const CharsetProvider& getStandardProvider() {
	static FastCharsetProvider provider;
	return provider;
}
const Charset& getDefaultCharset() {
	try {
		//return Charset::forName("UTF-8");
		return Charset::forName("US-ASCII");
	} catch(const Exception& e) {
		e.printStackTrace();
	}
	return (const Charset&)Object::null_obj;
}

class NullObject : extends Object {
public:
	NullObject() {}
	virtual ~NullObject() {}
	virtual String toString() const;
};

String name_NullObj = "null_obj";
String NullObject::toString() const { return name_NullObj; }
NullObject nullObject;
}

namespace lang {

Object& Object::null_obj = nullObject;

The_System::The_System() {
	Thread::currentThread();
}

const Logger SystemLog;
const The_System System;
const The_Math Math;

Properties The_System::props;
io::InputStream& The_System::in = std_in();
io::PrintStream& The_System::out = std_out();
io::PrintStream& The_System::err = std_err();

const util::Map<String,String>& The_System::getenv() {
	//return ProcessEnvironment.getenv();
	return get_env();
}
}

namespace nio {
namespace charset {
const CharsetProvider& Charset::standardProvider = getStandardProvider();
const Charset& Charset::defCharset = getDefaultCharset();
Array<String> CharsetEncoder::stateNames; // = { "RESET", "CODING", "CODING_END", "FLUSHED" };
}
namespace channels {
Object SelectorProvider::lock;
}
}
