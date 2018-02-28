#ifndef __NIO_CHANNELS_CHANNEL_HPP
#define __NIO_CHANNELS_CHANNEL_HPP

#include <io/Closeable.hpp>
#include <net/SocketOption.hpp>
#include <nio/ByteBuffer.hpp>

namespace nio {
namespace channels {

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
	virtual NetworkChannel& bind(SocketAddress local) = 0;
	virtual SocketAddress getLocalAddress() const = 0;
	template<class T>
	NetworkChannel& setOption(SocketOption<T>& name, T value) { return *this; }
	template<class T>
	T getOption(SocketOption<T>& name) const { T v; return v; }
};

class Selector;
class SelectorProvider;
class SelectionKey;
class SelectableChannel : implements Channel {
protected:
	SelectableChannel() {}

public:
	virtual Shared<SelectorProvider> provider() = 0;
	virtual int validOps() = 0;
	virtual boolean isRegistered() = 0;
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
	Shared<SelectorProvider> provider;
};

class ServerSocketChannel : extends AbstractSelectableChannel, implements NetworkChannel {
};

class SocketChannel : extends AbstractSelectableChannel, implements ByteChannel, implements NetworkChannel {
};

class UdpSocketChannel : extends SocketChannel {
};

class UnixSocketChannel : extends SocketChannel {
};

class TcpSocketChannel : extends SocketChannel {
};

}}

#endif
