#ifndef __NET_SOCKET_HPP
#define __NET_SOCKET_HPP

#include <lang/String.hpp>
#include <io/Closeable.hpp>
#include <io/IOException.hpp>

namespace net {

class UnknownHostException : extends io::IOException {
	using io::IOException::IOException;
};

class Socket : implements io::Closeable {
};

class DatagramSocket : implements io::Closeable {
};

class SocketAddress : extends Object {
};

class InetAddress : extends Object {
private:
	static const int IPv4 = 1;
	static const int IPv6 = 2;
public:
	static Shared<InetAddress> getByAddress(String host, const Array<byte>& addr);
	static Shared<InetAddress> getByName(String host);
	static Array<InetAddress> getAllByName(String host);
	static Shared<InetAddress> getLoopbackAddress();
	static Shared<InetAddress> getByAddress(const Array<byte>& addr) { return getByAddress("", addr); }
	static Shared<InetAddress> getLocalHost();
	static Shared<InetAddress> anyLocalAddress();

	virtual boolean isMulticastAddress() { return false; }
	virtual boolean isAnyLocalAddress() { return false; }
	virtual boolean isLoopbackAddress() { return false; }
	virtual boolean isLinkLocalAddress() { return false; }
	virtual boolean isSiteLocalAddress() { return false; }
};

class InetSocketAddress : extends SocketAddress {
private:
	String hostname;
	Shared<InetAddress> addr;
	int port;

	static int checkPort(int port);
	static String checkHost(const String& hostname);
public:
	InetSocketAddress(int port) : InetSocketAddress(InetAddress::anyLocalAddress(), port) {
	}
	InetSocketAddress(const InetAddress& addr, int port) {
		this->addr = addr ? addr : InetAddress.anyLocalAddress();
		this->port = checkPort(port);
	}
	InetSocketAddress(const String& hostname, int port) {
		this->hostname = checkHost(hostname);
		this->port = checkPort(port);
		try {
			this->addr = InetAddress::getByName(hostname);
		} catch(const UnknownHostException& e) {}
	}
	virtual int getPort() final { return port; }
	virtual InetAddress getAddress() final { return addr; }
	virtual String getHostName() final { return hostname; }
};

}

#endif
