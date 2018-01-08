#include <lang/String.hpp>
#include <nio/CoderResult.hpp>

namespace {
const char* names[] = { "UNDERFLOW", "OVERFLOW", "MALFORMED", "UNMAPPABLE" };
}

namespace nio {

CoderResult CoderResult::UNDERFLOW = CoderResult(CoderResult::CR_UNDERFLOW, 0);
CoderResult CoderResult::OVERFLOW = CoderResult(CoderResult::CR_OVERFLOW, 0);

String CoderResult::toString() {
	String nm = names[type];
	return isError() ? nm + "[" + length + "]" : nm;
}

}
