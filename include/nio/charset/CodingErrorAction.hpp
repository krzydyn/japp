#ifndef __NIO_CHARSET_CODINGERRORACTION_HPP
#define __NIO_CHARSET_CODINGERRORACTION_HPP

#include <lang/String.hpp>

namespace nio {
namespace charset {

class CodingErrorAction : extends Object {
private:
	String name;
	CodingErrorAction(const String& name) : name(name) {}
public:
	static CodingErrorAction IGNORE;
	static CodingErrorAction REPLACE;
	static CodingErrorAction REPORT;
	String toString() const {return name;}
};

}}

#endif
