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
protected:
	static const int IPv4 = 1;
	static const int IPv6 = 2;

	String hostName;
	String originalHostName;
	int address;
	int family;
public:
	static Shared<InetAddress> getByAddress(const String& host, const Array<byte>& addr);
	static Shared<InetAddress> getByName(const String& host);
	static Array<Shared<InetAddress>> getAllByName(const String& host);
	static Shared<InetAddress> getLoopbackAddress();
	static Shared<InetAddress> getByAddress(const Array<byte>& addr) { return getByAddress("", addr); }
	static Shared<InetAddress> getLocalHost();
	static Shared<InetAddress> anyLocalAddress();

	virtual boolean isMulticastAddress() { return false; }
	virtual boolean isAnyLocalAddress() { return false; }
	virtual boolean isLoopbackAddress() { return false; }
	virtual boolean isLinkLocalAddress() { return false; }
	virtual boolean isSiteLocalAddress() { return false; }

	String getHostAddress() const { return "null"; }
	String getHostName() const { return getHostName(true); }
	String getHostName(boolean check) const {
		return hostName;
	}

	boolean equals(const Object& obj) const { return false; }
	String toString() const {
		return ((hostName != null) ? hostName : "") + "/" + getHostAddress();
	}
};

class InetSocketAddress : extends SocketAddress {
private:
	String hostname;
	Shared<InetAddress> addr;
	int port;

	static int checkPort(int port);
	static const String& checkHost(const String& hostname);

	boolean isUnresolved() const { return addr == null; }
public:
	InetSocketAddress() {}
	InetSocketAddress(int port) : InetSocketAddress(InetAddress::anyLocalAddress(), port) {
	}
	InetSocketAddress(Shared<InetAddress> addr, int port) {
		this->addr = addr ? addr : InetAddress::anyLocalAddress();
		this->port = checkPort(port);
	}
	InetSocketAddress(const String& hostname, int port) {
		this->hostname = checkHost(hostname);
		this->port = checkPort(port);
		try {
			this->addr = InetAddress::getByName(hostname);
		} catch(const UnknownHostException& e) {}
	}

	virtual int getPort() const final { return port; }
	virtual const InetAddress& getAddress() const final { return *addr; }
	virtual const String& getHostName() const final { return hostname; }

	String toString() const {
		if (isUnresolved()) return hostname + ":" + port;
		return addr->toString() + ":" + port;
	}
};

class Inet4Address final : extends InetAddress {
public:
	Inet4Address() {
		hostName = null;
		address = 0;
		family = IPv4;
	}
};

}

#endif
