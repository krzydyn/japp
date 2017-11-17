#ifndef __UTIL_STRINGTOKENIZER_HPP
#define __UTIL_STRINGTOKENIZER_HPP

#include <lang/Object.hpp>

namespace util {

class StringTokenizer : extends Object {
private:
	String str;
public:
	StringTokenizer(const String& str) : str(str) {}
	boolean hasMoreTokens() {return false;}
	String nextToken() { return ""; }
	String nextToken(const String& delim) { return ""; }
	boolean hasMoreElements() {return false;}
	Object* nextElement() {return null;}
	int countTokens() {return 0;}
};

}

#endif
