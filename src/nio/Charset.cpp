#include <nio/charset/Charset.hpp>
#include <util/HashMap.hpp>

namespace nio {
namespace charset {

namespace {

class UTF_8 : extends Charset {
public:
	UTF_8() : Charset("UTF-8",Array<String>()) {}
	String historicalName() { return "UTF8"; }
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
		//TODO don't use String::toLowerCase, it base on Charset
		return s.toLowerCase();
	}

	HashMap<String,String> aliasMap;

	const String& canonicalize(const String& csn) const {
		std::cout << "fast::canonicalize " << csn.intern() << std::endl;
		const String& acn = aliasMap.get(csn);
		return (acn != null_ref) ? acn : csn;
	}
	const Charset& lookup(const String& charsetName) const {TRACE;
		const String& csn = canonicalize(toLower(charsetName));
		std::cout << "fast::Lookup " << csn.intern() << std::endl;
		return (Charset&)null_ref;
	}
public:
	FastCharsetProvider() {
	}
	const Charset& charsetForName(const String& charsetName) const {TRACE;
		std::cout << "fast::charsetForName " << charsetName.intern() << std::endl;
		return lookup(charsetName);
	}
	SharedIterator<Charset> charsets() const {TRACE;
		return null;
	}
};

class StandardCharsets : extends FastCharsetProvider {
};

StandardCharsets the_standardProvider;
const Charset& getDefaultCharset() {
	try {
		return Charset::forName("UTF-8");
	} catch(const Exception& e) {
		e.printStackTrace();
		exit(1);
	}
}
}

const CharsetProvider& Charset::standardProvider = the_standardProvider;
const Charset& Charset::defCharset = getDefaultCharset();

const Charset& Charset::lookup(const String& charsetName) {
	std::cout << "Charset::lookup " << charsetName.intern() << std::endl;
	const Charset& cs = standardProvider.charsetForName(charsetName);
	return cs;
}

Array<String> CharsetEncoder::stateNames; // = { "RESET", "CODING", "CODING_END", "FLUSHED" };

}} //namespace io::charset
