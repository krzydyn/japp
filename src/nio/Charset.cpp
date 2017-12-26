#include <lang/System.hpp>
#include <nio/charset/Charset.hpp>

namespace nio {
namespace charset {

const Charset& Charset::lookup(const String& charsetName) {
	System.out.println("Charset::lookup(" + charsetName + ")");
	const Charset& cs = standardProvider.charsetForName(charsetName);
	return cs;
}


}} //namespace io::charset
