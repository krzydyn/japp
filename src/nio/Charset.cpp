#include <lang/System.hpp>
#include <nio/CoderResult.hpp>
#include <nio/charset/Charset.hpp>
#include <nio/charset/CodingErrorAction.hpp>

namespace {
const char* names[] = { "UNDERFLOW", "OVERFLOW", "MALFORMED", "UNMAPPABLE" };
}

namespace nio {

CoderResult CoderResult::UNDERFLOW = CoderResult(CoderResult::CR_UNDERFLOW, 0);
CoderResult CoderResult::OVERFLOW = CoderResult(CoderResult::CR_OVERFLOW, 0);

String CoderResult::toString() const {
	String nm = names[mType];
	return isError() ? nm + "[" + mLength + "]" : nm;
}


namespace charset {

const Charset& Charset::lookup(const String& charsetName) {
	System.out.println("Charset::lookup(" + charsetName + ")");
	const Charset& cs = standardProvider.charsetForName(charsetName);
	return cs;
}

CodingErrorAction CodingErrorAction::IGNORE = CodingErrorAction("IGNORE");
CodingErrorAction CodingErrorAction::REPORT = CodingErrorAction("REPORT");
CodingErrorAction CodingErrorAction::REPLACE = CodingErrorAction("REPLACE");

}} //namespace io::charset
