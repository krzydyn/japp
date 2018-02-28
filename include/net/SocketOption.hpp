#ifndef __NET_SOCKETOPTION_HPP
#define __NET_SOCKETOPTION_HPP

#include <lang/String.hpp>
#include <lang/Class.hpp>

namespace nio {
namespace channels {

template<class T>
interface SocketOption : Interface {
public:
	virtual const String& name() = 0;
	virtual Class type() = 0;
};

}}

#endif
