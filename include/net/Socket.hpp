#ifndef __NET_SOCKET_HPP
#define __NET_SOCKET_HPP

#include <io/Closeable.hpp>

namespace net {

class Socket : implements io::Closeable {
};

class DatagramSocket : implements io::Closeable {
};

}

#endif
