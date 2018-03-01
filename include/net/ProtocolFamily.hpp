#ifndef __NET_PROTOCOLFAMILY_HPP
#define __NET_PROTOCOLFAMILY_HPP

#include <lang/Object.hpp>

namespace net {

interface ProtocolFamily : Interface {
public:
	String name();
};

}

#endif
