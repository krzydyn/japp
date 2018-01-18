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
	Log.log("CharsetDecoder::decode in: %s  out: %s", in.toString().cstr(), out.toString().cstr());
	for (;;) {
		CoderResult cr = decodeLoop(in, out);
		Log.log("CharsetDecoder::decodeLoop = %s",cr.toString().cstr());
		if (cr.isOverflow()) return cr;
		if (cr.isUnderflow()) {
			if (endOfInput && in.hasRemaining()) {
				cr = CoderResult::malformedForLength(in.remaining());
			}
			else {
				return cr;
			}
		}
	}
	out.flip();
	return CoderResult::UNDERFLOW;
}

}}
