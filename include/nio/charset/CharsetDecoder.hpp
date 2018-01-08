#ifndef __NIO_CHARSET_CHARSETDECODER_HPP
#define __NIO_CHARSET_CHARSETDECODER_HPP

#include <lang/String.hpp>
#include <nio/CoderResult.hpp>
#include <nio/charset/Charset.hpp>

namespace nio {
namespace charset {

class CharsetDecoder : extends Object {
private:
	Charset *mCharset;
	const float mAverageCharsPerByte;
	const float mMaxCharsPerByte;
	String mReplacement;

	// Internal states
	static const int ST_RESET   = 0;
	static const int ST_CODING  = 1;
	static const int ST_END     = 2;
	static const int ST_FLUSHED = 3;

	int state = ST_RESET;

	CharsetDecoder(Charset *cs, float averageCharsPerByte, float maxCharsPerByte, const String& replacement) :
			mCharset(cs),
			mAverageCharsPerByte(averageCharsPerByte),
			mMaxCharsPerByte(maxCharsPerByte) {
			replaceWith(replacement);
	}

protected:
	CharsetDecoder(Charset *cs, float averageCharsPerByte, float maxCharsPerByte) :
        CharsetDecoder(cs, averageCharsPerByte, maxCharsPerByte, "\uFFFD") {
    }

	virtual void implReplaceWith(const String& newReplacement) {}
	virtual void implReset() {}

public:
	Charset& charset() { return *mCharset; }
	const String& replacement() { return mReplacement; }
	virtual CharsetDecoder& replaceWith(const String& newReplacement) final {
		mReplacement = newReplacement;
        implReplaceWith(newReplacement);
		return *this;
	}
	virtual CharsetDecoder& reset() final {
        implReset();
		state = ST_RESET;
		return *this;
	}

	::nio::CoderResult decode(ByteBuffer& in, CharBuffer& out, boolean endOfInput) {
		//out.put(in);
		out.flip();
		return CoderResult::UNDERFLOW;
	}
};

}}

#endif
