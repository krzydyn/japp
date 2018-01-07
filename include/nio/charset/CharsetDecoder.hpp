#ifndef __NIO_CHARSET_CHARSETDECODER_HPP
#define __NIO_CHARSET_CHARSETDECODER_HPP

#include <lang/String.hpp>
#include <nio/charset/Charset.hpp>

namespace nio {
namespace charset {

class CharsetDecoder : extends Object {
private:
	Charset *charset;
	const float averageCharsPerByte;
	const float maxCharsPerByte;
	String replacement;

	// Internal states
	static const int ST_RESET   = 0;
	static const int ST_CODING  = 1;
	static const int ST_END     = 2;
	static const int ST_FLUSHED = 3;

	int state = ST_RESET;

	CharsetDecoder(Charset *cs, float averageCharsPerByte, float maxCharsPerByte, const String& replacement) :
			charset(cs),
			averageCharsPerByte(averageCharsPerByte),
			maxCharsPerByte(maxCharsPerByte),
			replacement(replacement) {
	}
};

}}

#endif
