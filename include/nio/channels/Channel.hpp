#ifndef __NIO_CHANNELS_CHANNEL_HPP
#define __NIO_CHANNELS_CHANNEL_HPP

#include <io/Closeable.hpp>
#include <net/Socket.hpp>
#include <net/SocketOption.hpp>
#include <nio/ByteBuffer.hpp>
#include <nio/channels/SelectorProvider.hpp>

namespace nio {
namespace channels {

class ClosedChannelException : extends io::IOException {
	using io::IOException::IOException;
};

class NotYetConnectedException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class UnsupportedAddressTypeException : extends IllegalArgumentException {
	using IllegalArgumentException::IllegalArgumentException;
};

class AlreadyBoundException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class UnresolvedAddressException : extends IllegalArgumentException {
	using IllegalArgumentException::IllegalArgumentException;
};


class SelectionKey : extends Object {
private:
	Object* mAttachment = null;
protected:
	SelectionKey() { }
public:
	static const int OP_READ = 1 << 0;
	static const int OP_WRITE = 1 << 2;
	static const int OP_CONNECT = 1 << 3;
	static const int OP_ACCEPT = 1 << 4;

	virtual Selector& selector() = 0;
	virtual boolean isValid() = 0;
	virtual void cancel() = 0;
	virtual int interestOps() = 0;
	virtual SelectionKey& interestOps(int ops) = 0;
	virtual int readyOps() = 0;
	virtual boolean isReadable() final {
		return (readyOps() & OP_READ) != 0;
	}
	virtual boolean isWritable() final {
		return (readyOps() & OP_WRITE) != 0;
	}
	virtual boolean isConnectable() final {
		return (readyOps() & OP_CONNECT) != 0;
	}
	virtual boolean isAcceptable() final {
		return (readyOps() & OP_ACCEPT) != 0;
	}
	virtual Object* attach(Object* ob) final {
		mAttachment = ob;
		return ob;
	}
	virtual Object* attachment() final {
		return mAttachment;
	}
};

interface Channel : extends io::Closeable {
public:
	virtual boolean isOpen() const = 0;
	virtual void close() = 0;
};

interface ReadableByteChannel : implements Channel {
public:
	virtual int read(ByteBuffer& dst) = 0;
};

interface WritableByteChannel : implements Channel {
public:
	virtual int write(ByteBuffer& src) = 0;
};

interface ScatteringByteChannel : implements ReadableByteChannel {
public:
	virtual long read(Array<ByteBuffer>& dsts, int offset, int length) = 0;
	virtual long read(Array<ByteBuffer>& dsts) = 0;
};

interface GatheringByteChannel : implements WritableByteChannel {
	virtual long write(Array<ByteBuffer>& srcs, int offset, int length) = 0;
	virtual long write(Array<ByteBuffer>& srcs) = 0;
};

interface ByteChannel : implements ReadableByteChannel, implements WritableByteChannel {
};

interface NetworkChannel : implements Channel {
	virtual NetworkChannel& bind(const SocketAddress& local) = 0;
	virtual const SocketAddress& getLocalAddress() const = 0;
	virtual NetworkChannel& setOption(const SocketOption& name, Object* value) = 0;
	virtual Object* getOption(const SocketOption& name) const = 0;
};

class NetworkInterface;
class MembershipKey;
interface MulticastChannel : implements NetworkChannel {
	//virtual void close() = 0;
	virtual Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf) = 0;
	virtual Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf, const InetAddress& source) = 0;
};

class AbstractInterruptibleChannel : extends Object, implements Channel {
private:
	Object closeLock;
	boolean mIsOpen = true;
protected:
	static void blockedOn(Interruptible& intr) {}
	AbstractInterruptibleChannel() {}

	virtual void begin() final {
	}
	virtual void end(boolean completed) final {
	}
	virtual void implCloseChannel() = 0;
public:
	virtual void close() final {
		synchronized (closeLock) {
			if (!mIsOpen) return;
			mIsOpen = false;
			implCloseChannel();
		}
	}
	virtual boolean isOpen() const final {
		return mIsOpen;
	}
};

class SelectableChannel : extends AbstractInterruptibleChannel {
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
	Object keyLock;
	Object regLock;
	boolean blocking = true;
	Array<SelectionKey*> keys;
	int keyCount = 0;

	void addKey(SelectionKey& k) {
		int i = 0;
		if (keys.length == 0) {
			keys = Array<SelectionKey*>(3);
		}
		else if (keyCount < keys.length) {
			for (i = 0; i < keys.length; ++i) {
				if (keys[i] == null) break;
			}
		}
		else {
			int n = keys.length * 2;
			Array<SelectionKey*> ks(n);
			for (i = 0; i < keys.length; ++i) {
				ks[i] = keys[i];
			}
			keys = ks;
			i = keyCount;
		}
		keys[i] = &k;
		++keyCount;
	}
	SelectionKey& findKey(Selector& sel) const {
		synchronized (keyLock) {
			if (keys.length == 0) return (SelectionKey&)null_obj;
			for (int i = 0; i < keys.length; i++) {
				if ((keys[i] != null) && (keys[i]->selector() == sel))
					return *keys[i];
			}
		}
		return (SelectionKey&)null_obj;
	}
	void removeKey(SelectionKey& k) {
		synchronized (keyLock) {
			for (int i = 0; i < keys.length; i++) {
				if ((keys[i] != &k)) {
					keys[i] = null;
					--keyCount;
				}
			}
			//k.invalidate();
			k.cancel();
		}
	}
	boolean haveValidKeys() {
		synchronized (keyLock) {
		}
		return false;
	}
	
protected:
	AbstractSelectableChannel(Shared<SelectorProvider> provider) : mProvider(provider) {
	}
public:
	virtual Shared<SelectorProvider> provider() final { return mProvider; }
	virtual boolean isRegistered() const final {
		synchronized (keyLock) {
			return keyCount != 0;
		}
		return false;
	}
	SelectionKey& keyFor(Selector& sel) const {
		return findKey(sel);
	}
	SelectionKey& registerChn(Selector& sel, int ops, Object* att) {
		synchronized (regLock) {
			SelectionKey& k = findKey(sel);
			if (k != null) {
				k.interestOps(ops);
				k.attach(att);
			}
			else {
				synchronized (keyLock) {
				}
			}
		}
		return (SelectionKey&)null_obj;
	}
	boolean isBlocking() const final {
		synchronized (regLock) {
			return blocking;
		}
		return false;
	}
	Object& blockingLock() final {
		return regLock;
	}
	SelectableChannel& configureBlocking(boolean block) final {
		return *this;
	}
};

// Datagram (UDP)
class DatagramChannel : extends AbstractSelectableChannel, implements ByteChannel,
		implements ScatteringByteChannel, implements GatheringByteChannel {
protected:
	DatagramChannel(Shared<SelectorProvider> provider) : AbstractSelectableChannel(provider) {
	}
public:
	static Shared<DatagramChannel> open();
	static Shared<DatagramChannel> open(const ProtocolFamily& family);

	virtual int validOps() const final { return SelectionKey::OP_READ | SelectionKey::OP_WRITE; }
	virtual DatagramChannel& bind(const SocketAddress& local) = 0;
	virtual DatagramChannel& setOption(const SocketOption& name, Object* value) = 0;
	virtual DatagramSocket& socket() = 0;
	virtual boolean isConnected() = 0;
	virtual DatagramChannel& connect(const SocketAddress& remote) = 0;
	virtual DatagramChannel& disconnect() = 0;
	virtual const SocketAddress& getRemoteAddress() const = 0;
	virtual const SocketAddress& getLocalAddress() const = 0;
	virtual const SocketAddress& receive(ByteBuffer& dst) = 0;
	virtual int send(ByteBuffer& src, const SocketAddress& target) = 0;
	virtual int read(ByteBuffer& dst) = 0;
	virtual long read(Array<ByteBuffer>& dsts, int offset, int length) = 0;
	virtual long read(Array<ByteBuffer>& dsts) final {
		return read(dsts, 0, dsts.length);
	}
	virtual int write(ByteBuffer& src) = 0;
	virtual long write(Array<ByteBuffer>& srcs, int offset, int length) = 0;
	virtual long write(Array<ByteBuffer>& srcs) final {
		return write(srcs, 0, srcs.length);
	}
};

// TCP Server socket
class ServerSocketChannel : extends AbstractSelectableChannel, implements NetworkChannel {
};

// TCP Client socket
class SocketChannel : extends AbstractSelectableChannel,
		implements ByteChannel, implements ScatteringByteChannel, implements GatheringByteChannel,
		implements NetworkChannel {
protected:
	SocketChannel(Shared<SelectorProvider> provider) : AbstractSelectableChannel(provider) {
	}
public:
	static Shared<SocketChannel> open();
	static Shared<SocketChannel> open(SocketAddress remote);

	int validOps() const final { return SelectionKey::OP_READ | SelectionKey::OP_WRITE | SelectionKey::OP_CONNECT; }
	SocketChannel& bind(const SocketAddress& local) = 0;
	SocketChannel& setOption(const SocketOption& name, Object* value) = 0;
	virtual SocketChannel& shutdownInput() = 0;
	virtual SocketChannel& shutdownOutput() = 0;
	virtual Shared<Socket> socket() = 0;
	virtual boolean isConnected() = 0;
	virtual boolean isConnectionPending() = 0;
	virtual boolean connect(const SocketAddress& remote) = 0;
	virtual boolean finishConnect() = 0;
	virtual const SocketAddress& getRemoteAddress() const = 0;
	virtual const SocketAddress& getLocalAddress() const = 0;
	int read(ByteBuffer& dst) = 0;
	long read(Array<ByteBuffer>& dsts, int offset, int length) = 0;
	long read(Array<ByteBuffer>& dsts) final {
		return read(dsts, 0, dsts.length);
	}
	int write(ByteBuffer& src) = 0;
	long write(Array<ByteBuffer>& srcs, int offset, int length) = 0;
	long write(Array<ByteBuffer>& srcs) final {
		return write(srcs, 0, srcs.length);
	}
};

// Unix Domain socket
class UnixSocketChannel : extends AbstractSelectableChannel, implements ByteChannel {
};

}}

#endif
