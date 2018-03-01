#include <nio/channels/Channel.hpp>

namespace nio {
namespace channels {

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
	SelectorImpl(SelectorProvider& sp) : AbstractSelector(sp) {
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
	AbstractPollSelectorImpl(SelectorProvider& sp, int channels, int offset) : SelectorImpl(sp),
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
	PollSelectorImpl(SelectorProvider& p) : AbstractPollSelectorImpl(p, 1, 1) {
	}
};

class PollSelectorProviderImpl : extends SelectorProvider {
public:
	virtual Shared<DatagramChannel> openDatagramChannel() {
		return null;
	}
	virtual Shared<DatagramChannel> openDatagramChannel(ProtocolFamily family) {
		return null;
	}
	virtual Shared<Pipe> openPipe() {
		return null;
	}
	virtual Shared<AbstractSelector> openSelector() {
		return makeShared<PollSelectorImpl>(*this);
	}
	virtual Shared<ServerSocketChannel> openServerSocketChannel() {
		return null;
	}
	virtual Shared<SocketChannel> openSocketChannel() {
		return null;
	}
	virtual Shared<Channel> inheritedChannel() {
		//return InheritedChannel::getChannel();
		return null;
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
	return makeShared<PollSelectorProviderImpl>();
}

Shared<SelectorProvider> SelectorProvider::mProvider = null;

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

Shared<SocketChannel> SocketChannel::open() {
	return SelectorProvider::provider()->openSocketChannel();
}

Shared<DatagramChannel> DatagramChannel::open() {
	return SelectorProvider::provider()->openDatagramChannel();
}

}}
