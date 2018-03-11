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

class InetAddress : extends Object {
protected:
	static const int IPv4 = 1;
	static const int IPv6 = 2;

	String hostName;
	String originalHostName;
	int family;

public:
	static Shared<InetAddress> getByAddress(const String& host, const Array<byte>& addr);
	static Shared<InetAddress> getByName(const String& host);
	static Array<Shared<InetAddress>> getAllByName(const String& host);
	static Shared<InetAddress> getLoopbackAddress();
	static Shared<InetAddress> getByAddress(const Array<byte>& addr) { return getByAddress("", addr); }
	static Shared<InetAddress> getLocalHost();
	static Shared<InetAddress> anyLocalAddress();

	virtual boolean isMulticastAddress() const { return false; }
	virtual boolean isAnyLocalAddress() const { return false; }
	virtual boolean isLoopbackAddress() const { return false; }
	virtual boolean isLinkLocalAddress() const { return false; }
	virtual boolean isSiteLocalAddress() const { return false; }

	virtual Array<byte> getAddress() const = 0;
	virtual String getHostAddress() const { return ""; }
	virtual String getHostName() const { return getHostName(true); }
	virtual String getHostName(boolean check) const { return hostName; }

	boolean equals(const Object& obj) const { return false; }
	String toString() const {
		String a = getHostAddress();
		if (a.length() > 0) a = "/" + a;
		return ((hostName != null) ? hostName : "") + a;
	}
};
class Inet4Address final : extends InetAddress {
	static const int INADDRSZ = 4;
	uint32_t address;
	static String numericToTextFormat(uint32_t address);
public:
	Inet4Address() {
		hostName = "";
		family = IPv4;
		address = 0;
	}
	Inet4Address(const String& hostName, const Array<byte>& addr) {
		this->hostName = hostName;
		this->family = IPv4;
		if (addr.length == INADDRSZ) {
			int address  = addr[3] & 0xFF;
			address |= ((addr[2] << 8) & 0xFF00);
			address |= ((addr[1] << 16) & 0xFF0000);
			address |= ((addr[0] << 24) & 0xFF000000);
			this->address = address;
		}
		originalHostName = hostName;
	}
	Inet4Address(const String& hostName, uint32_t address) {
		this->hostName = hostName;
		this->family = IPv4;
		this->address = address;
		originalHostName = hostName;
	}
	Array<byte> getAddress() const {
		Array<byte> addr(INADDRSZ);
		addr[0] = (byte) ((address >> 24) & 0xFF);
		addr[1] = (byte) ((address >> 16) & 0xFF);
		addr[2] = (byte) ((address >> 8) & 0xFF);
		addr[3] = (byte) (address & 0xFF);
		return addr;
	}
	String getHostAddress() const {
		return numericToTextFormat(address);
	}
};


class SocketAddress : extends Object {
};
class InetSocketAddress : extends SocketAddress {
private:
	String hostname;
	Shared<InetAddress> addr;
	int port;

	static int checkPort(int port);
	static const String& checkHost(const String& hostname);

public:
	InetSocketAddress() : addr(null), port(0) {}
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

	boolean isUnresolved() const { return addr == null; }

	virtual int getPort() const final { return port; }
	virtual const InetAddress& getAddress() const final { return addr ? *addr : (InetAddress&)null_obj; }
	virtual const String& getHostName() const final { return hostname; }

	String toString() const {
		if (isUnresolved()) return hostname + ":" + port;
		return addr->toString() + ":" + port;
	}
};

}

#endif
