#ifndef __NIO_CHARSET_CHARSETDECODER_HPP
#define __NIO_CHARSET_CHARSETDECODER_HPP

#include <lang/String.hpp>
#include <nio/CoderResult.hpp>
#include <nio/charset/Charset.hpp>
#include <nio/charset/CodingErrorAction.hpp>

namespace {
const char* stateNames[] = { "RESET", "CODING", "CODING_END", "FLUSHED" };
}

namespace nio {
class ByteBuffer;
class CharBuffer;

namespace charset {

class CharsetDecoder : extends Object {
private:
	const Charset *mCharset;
	const float mAverageCharsPerByte;
	const float mMaxCharsPerByte;
	String mReplacement;
	CodingErrorAction *mMalformedInputAction = &CodingErrorAction::REPORT;
	CodingErrorAction *mUnmappableCharacterAction = &CodingErrorAction::REPORT;

	// Internal states
	static const int ST_RESET   = 0;
	static const int ST_CODING  = 1;
	static const int ST_END     = 2;
	static const int ST_FLUSHED = 3;

	int state = ST_RESET;

	CharsetDecoder(const Charset *cs, float averageCharsPerByte, float maxCharsPerByte, const String& replacement) :
			mCharset(cs),
			mAverageCharsPerByte(averageCharsPerByte),
			mMaxCharsPerByte(maxCharsPerByte) {
		replaceWith(replacement);
	}

	void throwIllegalStateException(int from, int to) {
		throw IllegalStateException(String("Current state = ") + stateNames[from] + ", new state = " + stateNames[to]);
	}

protected:
	CharsetDecoder(const Charset *cs, float averageCharsPerByte, float maxCharsPerByte) :
		CharsetDecoder(cs, averageCharsPerByte, maxCharsPerByte, "\uFFFD") {
	}

	virtual void implReplaceWith(const String& newReplacement) {}
	virtual void implReset() {}
	virtual CoderResult decodeLoop(ByteBuffer& in, CharBuffer& out) = 0;

public:
	const Charset& charset() const { return *mCharset; }
	const String& replacement() const { return mReplacement; }
	virtual CharsetDecoder& replaceWith(const String& newReplacement) final {
		mReplacement = newReplacement;
		implReplaceWith(newReplacement);
		return *this;
	}
	const CodingErrorAction& malformedInputAction() { return *mMalformedInputAction; }
	virtual CharsetDecoder& reset() final {
		implReset();
		state = ST_RESET;
		return *this;
	}

	virtual float averageCharsPerByte() final { return mAverageCharsPerByte; }
	virtual float maxCharsPerByte() final { return mMaxCharsPerByte; }

	CoderResult decode(ByteBuffer& in, CharBuffer& out, boolean endOfInput);
};

}}

#endif
