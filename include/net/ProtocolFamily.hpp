#ifndef __NET_PROTOCOLFAMILY_HPP
#define __NET_PROTOCOLFAMILY_HPP

#include <lang/Object.hpp>

namespace net {

interface ProtocolFamily : Interface {
private:
	static int enumIndex;
protected:
	ProtocolFamily() : value(enumIndex++) {}
	const int value;
public:
	String name();
	boolean operator==(const ProtocolFamily& o) const { return this == &o; }
	operator int() const { return value; }
};

//inline boolean operator==(const ProtocolFamily& f1, const ProtocolFamily& f2) {
//	return &f1 == &f2;
//}

class StandardProtocolFamily : implements ProtocolFamily {
public:
	static const StandardProtocolFamily INET;
	static const StandardProtocolFamily INET6;
};

}

#endif
