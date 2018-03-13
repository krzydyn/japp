#ifndef __NET_PROTOCOLFAMILY_HPP
#define __NET_PROTOCOLFAMILY_HPP

#include <lang/Object.hpp>

namespace net {

//enum-like class
interface ProtocolFamily : Interface {
private:
	static int enumIndex;
protected:
	ProtocolFamily() : value(enumIndex++) {}
	ProtocolFamily(int v) : value(v) { enumIndex=v+1; }
	const int value;
public:
	String name();
	boolean operator==(const ProtocolFamily& o) const { return this->value == o.value; }
	operator int() const { return value; }
};

class StandardProtocolFamily : implements ProtocolFamily {
	StandardProtocolFamily(int v) : ProtocolFamily(v) {}
public:
	static const StandardProtocolFamily INET;
	static const StandardProtocolFamily INET6;
};

}

#endif
