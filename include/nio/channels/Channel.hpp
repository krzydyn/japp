#ifndef __NIO_CHANNELS_CHANNEL_HPP
#define __NIO_CHANNELS_CHANNEL_HPP

#include <io/Closeable.hpp>
#include <net/Socket.hpp>
#include <net/SocketOption.hpp>
#include <nio/ByteBuffer.hpp>
#include <nio/channels/SelectorProvider.hpp>

namespace nio {
namespace channels {

class SelectionKey {
protected:
	SelectionKey() { }
public:
	static const int OP_READ = 1 << 0;
	static const int OP_WRITE = 1 << 2;
	static const int OP_CONNECT = 1 << 3;
	static const int OP_ACCEPT = 1 << 4;
};

interface Channel : extends io::Closeable {
public:
	virtual boolean isOpen() = 0;
	virtual void close() = 0;
};

interface ReadableByteChannel : extends Channel {
public:
	virtual int read(ByteBuffer& dst) = 0;
};

interface WritableByteChannel : extends Channel {
public:
	virtual int write(ByteBuffer& dst) = 0;
};

interface ByteChannel : extends ReadableByteChannel, extends WritableByteChannel {
};

class SocketAddress;
interface NetworkChannel : extends Channel {
	virtual NetworkChannel& bind(const SocketAddress& local) = 0;
	virtual SocketAddress getLocalAddress() const = 0;
	virtual NetworkChannel& setOption(const SocketOption& name, Object* value) = 0;
	virtual Object* getOption(const SocketOption& name) const = 0;
};

class InetAddress;
class NetworkInterface;
class MembershipKey;
interface MulticastChannel : extends NetworkChannel {
	virtual void close() = 0;
	virtual Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf) = 0;
	virtual Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf, const InetAddress& source) = 0;
};

class SelectableChannel : implements Channel {
protected:
	SelectableChannel() {}

public:
	virtual Shared<SelectorProvider> provider() = 0;
	virtual int validOps() const = 0;
	virtual boolean isRegistered() const = 0;
	virtual SelectionKey& keyFor(Selector& sel) const = 0;
	virtual SelectionKey& registerChn(Selector& sel, int ops, Object* att) = 0;
	virtual SelectionKey& registerChn(Selector& sel, int ops) final {
		return registerChn(sel, ops, null);
	}
	virtual SelectableChannel& configureBlocking(boolean block) = 0;
	virtual boolean isBlocking() const = 0;
	virtual Object& blockingLock() = 0;
};

class AbstractSelectableChannel : extends SelectableChannel {
private:
	Shared<SelectorProvider> mProvider;
protected:
	AbstractSelectableChannel(Shared<SelectorProvider> provider) : mProvider(provider) {
	}
public:
	Shared<SelectorProvider> provider() { return mProvider; }
};

// TCP Server socket
class ServerSocketChannel : extends AbstractSelectableChannel, implements NetworkChannel {
};

// TCP Client socket
class SocketChannel : extends AbstractSelectableChannel, implements ByteChannel, implements NetworkChannel {
protected:
	SocketChannel(Shared<SelectorProvider> provider) : AbstractSelectableChannel(provider) {
	}
public:
	static Shared<SocketChannel> open();

	virtual SocketChannel& bind(const SocketAddress& local) = 0;
};

// Datagram (UDP)
class DatagramChannel : extends AbstractSelectableChannel, implements ByteChannel, implements MulticastChannel {
protected:
	DatagramChannel(Shared<SelectorProvider> provider) : AbstractSelectableChannel(provider) {
	}
public:
	static Shared<DatagramChannel> open();
	static Shared<DatagramChannel> open(ProtocolFamily family);

	virtual int validOps() const final { return SelectionKey::OP_READ | SelectionKey::OP_WRITE; }
	virtual DatagramChannel& bind(const SocketAddress& local) = 0;
	virtual DatagramChannel& setOption(SocketOption& name, Object* value) = 0;
	virtual DatagramSocket& socket() = 0;
	virtual boolean isConnected() = 0;
};

// Unix Domain socket
class UnixSocketChannel : extends AbstractSelectableChannel, implements ByteChannel {
};

}}

#endif
