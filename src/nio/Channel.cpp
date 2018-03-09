#include <nio/channels/Channel.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace nio {
namespace channels {

class IOStatus {
#undef EOF
public:
	static const int EOF = -1;              // End of file
	static const int UNAVAILABLE = -2;      // Nothing available (non-blocking)
	static const int INTERRUPTED = -3;      // System call interrupted
	static const int UNSUPPORTED = -4;      // Operation not supported
	static const int THROWN = -5;           // Exception thrown in JNI code
	static const int UNSUPPORTED_CASE = -6; // This case not supported
};

class Net {
public:
	static InetSocketAddress& getRevealedLocalAddress(InetSocketAddress& addr);
	static InetSocketAddress checkAddress(const SocketAddress& sa);
	static int connect(const ProtocolFamily& family, int fd, const InetAddress& remote, int remotePort);
	static void bind(const ProtocolFamily& family, int fd, const InetAddress& local, int localPort);
};
InetSocketAddress& Net::getRevealedLocalAddress(InetSocketAddress& addr) { return addr; }
int Net::connect(const ProtocolFamily& family, int fd, const InetAddress& remote, int remotePort) {
	if (family == StandardProtocolFamily::INET) return 1;
	return -1;
}
void Net::bind(const ProtocolFamily& family, int fd, const InetAddress& local, int localPort) {
	struct sockaddr_in addr_bind;
/*
	socklen_t slen = sizeof(addr_bind);
	if (getsockname(fd, (struct sockaddr*)&addr_bind, &slen) != -1) {
		throw io::IOException(String("getsockname: socket already bound to an address"));
	}
	Log.log("fd not bound, err = %s", strerror(errno));
*/
	memset(&addr_bind, 0, sizeof(addr_bind));
	addr_bind.sin_family = AF_INET;//(short)family;
	addr_bind.sin_port = htons((short)localPort);
	//Array<byte> addr = local.getAddress();
	//memcpy(&addr_bind.sin_addr.s_addr, &addr[0], addr.length);
	addr_bind.sin_addr.s_addr = htonl(INADDR_ANY);
	if (::bind(fd, (struct sockaddr*)&addr_bind, sizeof(addr_bind)) == -1) {
		if (errno == EINVAL) io::IOException(String("bind: socket already bound to an address"));
		throw io::IOException(String("bind: ")+strerror(errno)+" on :"+String::valueOf(localPort));
	}
	Log.log("fd=%d: bound to port %s:%d", fd, local.toString().cstr(), localPort);
}

InetSocketAddress Net::checkAddress(const SocketAddress& sa) {
	if (sa == null) throw NullPointerException();
	if (!(instanceof<InetSocketAddress>(&sa))) {
		 throw UnsupportedAddressTypeException(sa.getClass().getName());
	}
	InetSocketAddress& isa = (InetSocketAddress&)sa;
	if (isa.isUnresolved()) throw UnresolvedAddressException(isa.getHostName());
	//InetAddress& addr = isa.getAddress();
	return (InetSocketAddress&)sa;
}

Shared<SelectorProvider> SelectorProvider::mProvider = null;

class SelectorImpl : extends AbstractSelector {
private:
	int lockAndDoSelect(long timeout) {
		synchronized (*this) {
			if (!isOpen()) throw ClosedSelectorException();
			return doSelect(timeout);
		}
		return -1;
	}
protected:
	SelectorImpl(Shared<SelectorProvider> sp) : AbstractSelector(sp) {
	}
	virtual int doSelect(long timeout) = 0;
public:
	virtual int selectNow() {
		return lockAndDoSelect(0);
	}
	void implCloseSelector() {
		wakeup();
		synchronized (*this) {
		}
	}
	int select(long timeout) {
		if (timeout < 0) throw IllegalArgumentException("Negative timeout");
		return lockAndDoSelect((timeout == 0) ? -1 : timeout);
	}
	int select() {
		return select(0);
	}

	Selector& wakeup() = 0;
};

class AbstractPollSelectorImpl : extends SelectorImpl {
protected:
	int totalChannels;
	int channelOffset;
	AbstractPollSelectorImpl(Shared<SelectorProvider> sp, int channels, int offset) : SelectorImpl(sp),
			totalChannels(channels), channelOffset(offset) {
	}
	virtual int doSelect(long timeout) = 0;
	int updateSelectedKeys() {
		return 0;
	}
public:
	Selector& wakeup() {
		return *this;
	}
};

// http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/nio/ch/PollSelectorImpl.java
class PollSelectorImpl : extends AbstractPollSelectorImpl {
private:
protected:
	int doSelect(long timeout) {
		int numKeysUpdated = updateSelectedKeys();
		return numKeysUpdated;
	}
public:
	PollSelectorImpl(Shared<SelectorProvider> p) : AbstractPollSelectorImpl(p, 1, 1) {
	}
};

class DatagramChannelImpl : extends DatagramChannel {
private:
	static const int ST_UNINITIALIZED = -1;
	static const int ST_UNCONNECTED = 0;
	static const int ST_CONNECTED = 1;
	static const int ST_KILLED = 2;

	int fdVal = -1;
	const ProtocolFamily& family;

	long readerThread = 0;
	long writerThread = 0;

	Object readLock;
	Object writeLock;
	Object stateLock;

	int state = ST_UNINITIALIZED;

	InetSocketAddress mLocalAddress;
	InetSocketAddress mRemoteAddress;

	boolean reuseAddressEmulated;
	boolean isReuseAddress;

	int poll(int events, long timeout) {
		return 0;
	}

	void ensureOpen() const {
		if (!isOpen()) throw ClosedChannelException();
	}
	void ensureOpenAndUnconnected() const {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			if (state != ST_UNCONNECTED) throw IllegalStateException("Connect already invoked");
		}
	}
	void kill() {
		throw UnsupportedOperationException(__FUNCTION__);
	}

	int read(int fd, ByteBuffer& dst) {
		int pos = dst.position();
		int lim = dst.limit();
		int rem = (pos <= lim ? lim - pos : 0);
		Log.log("reading max %d from fd=%d", rem, fd);
		if (rem == 0) return 0;
		int flags = MSG_NOSIGNAL; // MSG_DONTWAIT | MSG_DONTROUTE
		int n = (int)::recv(fd, &dst.array()[pos], rem, flags);
		Log.log("read %d bytes from fd=%d", n, fd);
		if (n > 0)
			dst.position(pos + n);
		return n;
	}
// only for connected descriptor, but Datagram is not connected
/*
	int write(int fd, ByteBuffer& src) {
		int pos = src.position();
		int lim = src.limit();
		int rem = (pos <= lim ? lim - pos : 0);
		Log.log("writing %d bytes into fd=%d", rem, fd);
		if (rem == 0) return 0;
		int flags = MSG_NOSIGNAL; // MSG_DONTWAIT | MSG_DONTROUTE
		int n = (int)::send(fd, &src.array()[pos], rem, flags);
		if (n == -1) {
			Log.log("writing to fd=%d error=%d (%s)", fd, errno, strerror(errno));
		}
		else {
			Log.log("wrote %d bytes to fd=%d", n, fd);
			src.position(pos + n);
		}
		return n;
	}
*/

	int receive(int fd, ByteBuffer& dst, InetSocketAddress& sender) {
		int pos = dst.position();
		int lim = dst.limit();
		int rem = (pos <= lim ? lim - pos : 0);
		Log.log("receiving max %d from fd=%d", rem, fd);
		if (rem == 0) return 0;
		struct sockaddr_in addr_remote;
		socklen_t slen = sizeof(addr_remote);
		int flags = MSG_NOSIGNAL; // | MSG_DONTWAIT;// | MSG_DONTROUTE
		int n = (int)::recvfrom(fd, &dst.array()[pos], rem, flags, (struct sockaddr *)&addr_remote, &slen);
		if (n == -1) {
			Log.log("receiving to fd=%d error=%d (%s)", fd, errno, strerror(errno));
		}
		else {
			dst.position(pos + n);
			Log.log("recv %d bytes from fd=%d", n, fd);
			int port = ntohs(addr_remote.sin_port);
			sender = InetSocketAddress(makeShared<Inet4Address>(), port);
		}
		return n;
	}
	int send(int fd, ByteBuffer& src, InetSocketAddress& target) {
		int pos = src.position();
		int lim = src.limit();
		int rem = (pos <= lim ? lim - pos : 0);
		Log.log("sending %d bytes into fd=%d", rem, fd);
		if (rem == 0) return 0;
		struct sockaddr_in addr_remote;
		memset(&addr_remote, 0, sizeof(addr_remote));
		addr_remote.sin_family = (short)family;
		addr_remote.sin_port = htons((short)target.getPort());
		Array<byte> addr = target.getAddress().getAddress();
		memcpy(&addr_remote.sin_addr.s_addr, &addr[0], addr.length);
		int flags = MSG_NOSIGNAL; // MSG_DONTWAIT | MSG_DONTROUTE
		int n = (int)::sendto(fd, &src.array()[pos], rem, flags, (struct sockaddr *)&addr_remote, sizeof(addr_remote));
		if (n == -1) {
			Log.log("sending to fd=%d error=%d (%s)", fd, errno, strerror(errno));
		}
		else {
			Log.log("sent %d bytes to fd=%d", n, fd);
			src.position(pos + n);
		}
		return n;
	}

	//void drop(MembershipKeyImpl& key) { }
	InetSocketAddress last_sender;

protected:
	void implCloseChannel() {}
	void implConfigureBlocking(boolean block) {}

public:
	DatagramChannelImpl(Shared<SelectorProvider> p, const ProtocolFamily& family = StandardProtocolFamily::INET) :
			DatagramChannel(p), family(family) {
		fdVal = ::socket(family, SOCK_DGRAM, 0);
		if (fdVal == -1) throw io::IOException(String("Create datagram: ")+strerror(errno));
		Log.log("Datagram socket created, fd=%d", fdVal);
		state = ST_UNCONNECTED;
	}
	~DatagramChannelImpl() {
		if (fdVal != -1) {
			Log.log("%s: socket closing, fd=%d", __FUNCTION__, fdVal);
			::close(fdVal);
			fdVal = -1;
		}
		else {
			Log.log("%s: socket never opened", __FUNCTION__);
		}
	}
	DatagramSocket& socket() {
		return (DatagramSocket&)null_obj;
	}
	const SocketAddress& getLocalAddress() const {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			//return Net::getRevealedLocalAddress(mLocalAddress);
			return mLocalAddress;
		}
		return (SocketAddress&)null_obj;
	}
	const SocketAddress& getRemoteAddress() const {
        synchronized (stateLock) {
            if (!isOpen()) throw ClosedChannelException();
            return mRemoteAddress;
        }
		return (SocketAddress&)null_obj;
    }
	DatagramChannel& setOption(const SocketOption& name, Object *value) {
		//if (name == null) throw NullPointerException();
		synchronized (stateLock) {
			ensureOpen();
		}
		return *this;
	}
	Object* getOption(const SocketOption& name) const {
		//if (name == null) throw NullPointerException();
		synchronized (stateLock) {
			ensureOpen();
		}
		return null;
	}
	const SocketAddress& receive(ByteBuffer& dst) {
		if (dst.isReadOnly()) throw IllegalArgumentException("Read-only buffer");

		synchronized (readLock) {
			ensureOpen();
			if (!isOpen()) return (const SocketAddress&)null_obj;
			//if (localAddress() == null) bind(null); //bind to random address
			int n = 0;
			Finalize(readerThread = 0;end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			n = receive(fdVal, dst, last_sender);
			if (n == -1) return (const SocketAddress&)null_obj;
		}
		return last_sender;
	}
	int send(ByteBuffer& src, const SocketAddress& target) {
		synchronized (writeLock) {
			ensureOpen();
			InetSocketAddress isa = Net::checkAddress(target);
			synchronized (stateLock) {
				if (!isConnected()) {
					if (target == null) throw NullPointerException();
				}
				else {
					if (!target.equals(mRemoteAddress)) {
						throw IllegalArgumentException("Connected address not equal to target address");
					}
					return write(src);
				}
			}

			int n = 0;
			Finalize(writerThread = 0;end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			if (!isOpen()) return 0;
			n = send(fdVal, src, isa);
		}
		return 0;
	}
	int read(ByteBuffer& buf) {
		synchronized (readLock) {
			synchronized (stateLock) {
				ensureOpen();
				if (!isConnected()) throw NotYetConnectedException();
			}

			int n = 0;
			Finalize(readerThread = 0;end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			if (!isOpen()) return 0;
			n = read(fdVal, buf);
			return n;
		}
		return -1;
	}
	long read(Array<ByteBuffer>& dsts, int offset, int length) {
		return -1;
	}

	int write(ByteBuffer& buf) {
		synchronized (writeLock) {
			synchronized (stateLock) {
				ensureOpen();
				if (!isConnected()) throw NotYetConnectedException();
			}

			int n = 0;
			Finalize(writerThread = 0;end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			if (!isOpen()) return 0;
			n = send(fdVal, buf, mRemoteAddress);
			return n;
		}
		return -1;
	}
	long write(Array<ByteBuffer>& srcs, int offset, int length) {
		return -1;
	}

	DatagramChannel& bind(const SocketAddress& local) {
		System.out.println("binding " + local.toString());
		synchronized(readLock) {
			synchronized(writeLock) {
				synchronized (stateLock) {
					ensureOpen();
					if (mLocalAddress.getPort() != 0) throw AlreadyBoundException();
					InetSocketAddress isa;
					if (local == null) {
						if (family == StandardProtocolFamily::INET) {
							isa = InetSocketAddress(InetAddress::getByName("0.0.0.0"), 0);
						}
						else {
							isa = InetSocketAddress(0);
						}
					}
					else {
						isa = Net::checkAddress(local);
						if (family == StandardProtocolFamily::INET) {
							const InetAddress& addr = isa.getAddress();
							if (!(instanceof<Inet4Address>(&addr))) {
								throw UnsupportedAddressTypeException(addr.getClass().getName() + " is not Inet4Address");
							}
						}
						else {
							throw UnsupportedAddressTypeException("Family is not supported");
						}
					}
					Net::bind(family, fdVal, isa.getAddress(), isa.getPort());
					mLocalAddress = isa;
					//mLocalAddress = Net::localAddress(fd);
				}
			}
		}
		return *this;
	}
	boolean isConnected() {
		synchronized (stateLock) {
			return (state == ST_CONNECTED);
		}
		return false;
	}
	DatagramChannel& connect(const SocketAddress& sa) {
		System.out.println("connecting " + sa.toString());
		synchronized(readLock) {
			synchronized(writeLock) {
				synchronized (stateLock) {
					ensureOpenAndUnconnected();
					InetSocketAddress isa = Net::checkAddress(sa);
					int n = Net::connect(family, fdVal, isa.getAddress(), isa.getPort());
					if (n <= 0) throw Error();
					state = ST_CONNECTED;
					mRemoteAddress = isa;
					Log.log("fd=%d: connected to %s", fdVal, mRemoteAddress.toString().cstr());
					//sender = isa;
					//localAddress = Net::localAddress(fd);
				}
			}
		}
		return *this;
	}
	DatagramChannel& disconnect() {
		synchronized(readLock) {
			synchronized(writeLock) {
				synchronized (stateLock) {
					if (!isConnected() || !isOpen()) return *this;
					mRemoteAddress = InetSocketAddress(0);
					state = ST_UNCONNECTED;
				}
			}
		}
		return *this;
	}
	Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf) {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	Shared<MembershipKey> join(const InetAddress& group, const NetworkInterface& interf, const InetAddress& source) {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	
	//boolean translateReadyOps(int ops, int initialOps, SelectionKeyImpl sk) { }
};

class SocketChannelImpl : extends SocketChannel {
private:
	static const int ST_UNINITIALIZED = -1;
	static const int ST_UNCONNECTED = 0;
	static const int ST_PENDING = 1;
	static const int ST_CONNECTED = 2;
	static const int ST_KILLPENDING = 3;
	static const int ST_KILLED = 4;

	int fdVal = -1;

	long readerThread = 0;
	long writerThread = 0;

	Object readLock;
	Object writeLock;
	Object stateLock;

	int state = ST_UNINITIALIZED;

	InetSocketAddress mLocalAddress;
	InetSocketAddress mRemoteAddress;

	boolean reuseAddressEmulated;
	boolean isReuseAddress;

	// Input/Output open
	boolean isInputOpen = true;
	boolean isOutputOpen = true;
	boolean readyToConnect = false;

	boolean ensureReadOpen() {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			if (!isConnected()) throw NotYetConnectedException();
			//if (!isInputOpen) return false;
			return isInputOpen;
		}
		return false;
	}
	boolean ensureWriteOpen() {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			if (!isConnected()) throw NotYetConnectedException();
			//if (!isInputOpen) return false;
			return isOutputOpen;
		}
		return false;
	}
	void ensureOpenAndUnconnected() {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			if (state == ST_CONNECTED) throw AlreadyConnectedException();
			if (state == ST_PENDING) throw ConnectionPendingException();
		}
	}
	void readerCleanup() {
		synchronized (stateLock) {
			readerThread = 0;
			if (state == ST_KILLPENDING) kill();
		}
	}
	void writerCleanup() {
		synchronized (stateLock) {
			writerThread = 0;
			if (state == ST_KILLPENDING) kill();
		}
	}
	void kill() {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	int sendOutOfBandData(byte b) {
		return -1;
	}

	int read(int fd, ByteBuffer& dst) {
		return -1;
	}
	int write(int fd, ByteBuffer& src) {
		return -1;
	}
public:
	SocketChannelImpl(Shared<SelectorProvider> p) : SocketChannel(p) {
		fdVal = ::socket(AF_INET, SOCK_STREAM, 0);
		if (fdVal == -1) throw io::IOException(String("Create socket: ")+strerror(errno));
		Log.log("Socket created, fd=%d", fdVal);
		state = ST_UNCONNECTED;
	}
	SocketChannelImpl(Shared<SelectorProvider> p, const InetSocketAddress& remote) : SocketChannel(p) {
		fdVal = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (fdVal == -1) throw io::IOException(String("Create socket: ")+strerror(errno));
		Log.log("Socket created, fd=%d", fdVal);
		mRemoteAddress = remote;
		state = ST_CONNECTED;
	}

	const SocketAddress& getLocalAddress() const {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			return mLocalAddress;
		}
	}
	const SocketAddress& getRemoteAddress() const {
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
			return mRemoteAddress;
		}
		return (SocketAddress&)null_obj;
	}

	SocketChannel& setOption(const SocketOption& name, Object *value) {
		//if (name == null) throw NullPointerException();
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
		}
		return *this;
	}
	Object* getOption(const SocketOption& name) const {
		//if (name == null) throw NullPointerException();
		synchronized (stateLock) {
			if (!isOpen()) throw ClosedChannelException();
		}
		return null;
	}

	int read(ByteBuffer& buf) {
		synchronized (readLock) {
			if (!ensureReadOpen()) return -1;
			int n = 0;
			Finalize(end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			synchronized (stateLock) {
				if (!isOpen()) return 0;
			}
			n = read(fdVal, buf);
			return n;
		}
		return -1;
	}
	long read(Array<ByteBuffer>& dsts, int offset, int length) {
		return -1;
	}

	int write(ByteBuffer& buf) {
		synchronized (writeLock) {
			ensureWriteOpen();
			int n = 0;
			Finalize(writerCleanup();end((n > 0) || (n == IOStatus::UNAVAILABLE)););
			begin();
			synchronized (stateLock) {
				if (!isOpen()) return 0;
				// writerThread = NativeThread.current();
			}
			n = write(fdVal, buf);
			return n;
		}
		return -1;
	}
	long write(Array<ByteBuffer>& srcs, int offset, int length) {
		return -1;
	}
	SocketChannel& bind(const SocketAddress& local) {
		return *this;
	}
	boolean isConnected() {
		synchronized (stateLock) {
			return (state == ST_CONNECTED);
		}
		return false;
	}
	boolean isConnectionPending() {
		synchronized (stateLock) {
			return (state == ST_PENDING);
		}
		return false;
	}
	
};

class PollSelectorProviderImpl : extends SelectorProvider {
public:
	Shared<SelectorProvider> self;
	virtual Shared<DatagramChannel> openDatagramChannel() {
		return makeShared<DatagramChannelImpl>(self);
	}
	virtual Shared<DatagramChannel> openDatagramChannel(const ProtocolFamily& family) {
		return makeShared<DatagramChannelImpl>(self, family);
	}
	virtual Shared<Pipe> openPipe() {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	virtual Shared<AbstractSelector> openSelector() {
		return makeShared<PollSelectorImpl>(self);
	}
	virtual Shared<ServerSocketChannel> openServerSocketChannel() {
		throw UnsupportedOperationException(__FUNCTION__);
	}
	virtual Shared<SocketChannel> openSocketChannel() {
		return makeShared<SocketChannelImpl>(self);
	}
	virtual Shared<Channel> inheritedChannel() {
		//return InheritedChannel::getChannel();
		throw UnsupportedOperationException(__FUNCTION__);
    }
};

class DefaultSelectorProvider {
public:
	static Shared<SelectorProvider> create();
};
Shared<SelectorProvider> DefaultSelectorProvider::create() {
	//String osname = System::getProperty("os.name");
	//if (osname.equals("SunOS")) return createProvider("sun.nio.ch.DevPollSelectorProvider");
	//if (osname.equals("Linux")) return createProvider("sun.nio.ch.EPollSelectorProvider");
	Shared<PollSelectorProviderImpl> p = makeShared<PollSelectorProviderImpl>();
	p->self = p;
	return p;
}

Shared<SelectorProvider> SelectorProvider::provider() {
	synchronized (lock) {
		if (mProvider != null) return mProvider;
		mProvider = DefaultSelectorProvider::create();
	}
	return mProvider;
}

Shared<Selector> Selector::open() {
	return SelectorProvider::provider()->openSelector();
}

Shared<DatagramChannel> DatagramChannel::open() {
	return SelectorProvider::provider()->openDatagramChannel();
}

Shared<SocketChannel> SocketChannel::open() {
	return SelectorProvider::provider()->openSocketChannel();
}

}}
