#include <nio/charset/StandardCharsets.hpp>

namespace nio {
namespace charset {

const Charset& StandardCharsets::US_ASCII = Charset::forName("US-ASCII");
const Charset& StandardCharsets::ISO_8859_1 = Charset::forName("ISO-8859-1");
const Charset& StandardCharsets::UTF_8 = Charset::forName("UTF-8");
const Charset& StandardCharsets::UTF_16B = Charset::forName("UTF-16BE");
const Charset& StandardCharsets::UTF_16LE = Charset::forName("UTF-16LE");
const Charset& StandardCharsets::UTF_16 = Charset::forName("UTF-16");

}}
