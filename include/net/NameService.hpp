#ifndef __NET_NAMESERVICE_HPP
#define __NET_NAMESERVICE_HPP

#include <lang/String.hpp>

namespace net {

class InetAddress;
interface NameService : Interface {
public:
	virtual Array<Shared<InetAddress>> lookupAllHostAddr(const String& addr) = 0;
	virtual String getHostByAddr(Array<byte> addr) = 0;
};

}

#endif
