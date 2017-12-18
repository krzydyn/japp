#include <nio/charset/Charset.hpp>
#include <util/HashMap.hpp>

namespace nio {
namespace charset {

namespace {
HashMap<String,Charset*> charsetmap;

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

	const String& canonicalize(const String& csn) {
		String& acn = aliasMap.get(csn);
		return (acn != null_ref) ? acn : csn;
	}
	Charset* lookup(const String& charsetName) {TRACE;
		const String& csn = canonicalize(toLower(charsetName));
		std::cout << "fast::Lookup " << csn.intern() << std::endl;
		return null;
	}
public:
	Charset *charsetForName(const String& charsetName) {TRACE;
		std::cout << "fast::charsetForName " << charsetName.intern() << std::endl;
		return lookup(charsetName);
	}
	SharedIterator<Charset> charsets() {TRACE;
		return null;
	}
};

FastCharsetProvider standardCharsets;
}

CharsetProvider& Charset::standardProvider = standardCharsets;
Charset Charset::defCharset = Charset::forName("UTF-8");

Charset* Charset::lookup(const String& charsetName) {
	std::cout << "Charset::lookup " << charsetName.intern() << std::endl;
	Charset *cs = charsetmap.get(charsetName);
	if (cs == null) cs = standardProvider.charsetForName(charsetName);
	return cs;
}

Array<String> CharsetEncoder::stateNames; // = { "RESET", "CODING", "CODING_END", "FLUSHED" };

}} //namespace io::charset
