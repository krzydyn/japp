#include <nio/ByteBuffer.hpp>
#include <nio/CharBuffer.hpp>
#include <nio/charset/CharsetDecoder.hpp>

namespace nio {
namespace charset {

CoderResult CharsetDecoder::decode(ByteBuffer& in, CharBuffer& out, boolean endOfInput) {
	int newState = endOfInput ? ST_END : ST_CODING;
	if ((state != ST_RESET) && (state != ST_CODING) && !(endOfInput && (state == ST_END)))
		throwIllegalStateException(state, newState);
	state = newState;
	CoderResult cr = CoderResult::UNDERFLOW;
	for (;;) {
		cr = decodeLoop(in, out);
		Log.log("CharsetDecoder::decodeLoop = %s",cr.toString().cstr());
		if (cr.isOverflow()) return cr;
		if (cr.isUnderflow()) {
			if (endOfInput && in.hasRemaining()) {
				cr = CoderResult::malformedForLength(in.remaining());
			}
			else break;
		}

		const CodingErrorAction* action = null;
		if (cr.isMalformed())
			action = mMalformedInputAction;
		else if (cr.isUnmappable())
			action = mUnmappableCharacterAction;
		else throw Exception("Assertion");

		if (action == &CodingErrorAction::REPORT)
			break;

		if (action == &CodingErrorAction::REPLACE) {
			if (out.remaining() < mReplacement.length())
				return CoderResult::OVERFLOW;
			out.put(mReplacement);
		}
		if ((action == &CodingErrorAction::IGNORE) || (action == CodingErrorAction::REPLACE)) {
			// Skip erroneous input either way
			in.position(in.position() + cr.length());
			continue;
		}

		throw Exception("Assertion");
	}
	Log.log("CharsetDecoder::decode in: %s  out: %s", in.toString().cstr(), out.toString().cstr());
	return cr;
}

}}
