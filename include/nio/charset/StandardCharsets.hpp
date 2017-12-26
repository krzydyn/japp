#ifndef __NIO_STANDARDCHARSETS_HPP
#define __NIO_STANDARDCHARSETS_HPP

#include <nio/charset/Charset.hpp>

namespace nio {
namespace charset {

class StandardCharsets final : extends Object {
public:
	static const Charset& US_ASCII;
	static const Charset& ISO_8859_1;
	static const Charset& UTF_8;
	static const Charset& UTF_16B;
	static const Charset& UTF_16LE;
	static const Charset& UTF_16;
};

}} //namespace nio::charset

#endif
