#ifndef __NIO_CHANNELS_SELECTORPROVIDER_HPP
#define __NIO_CHANNELS_SELECTORPROVIDER_HPP

#include <lang/Object.hpp>
#include <net/ProtocolFamily.hpp>

namespace nio {
namespace channels {

class ClosedSelectorException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

using namespace net;

class Channel;
class DatagramChannel;
class Pipe;
class AbstractSelector;
class ServerSocketChannel;
class SocketChannel;
class SelectorProvider : extends Object {
private:
	static Object lock;
	static Shared<SelectorProvider> mProvider;
	static boolean loadProviderFromProperty();
	static boolean loadProviderAsService();

protected:
	SelectorProvider() {}

public:
	static Shared<SelectorProvider> provider();

	virtual Shared<DatagramChannel> openDatagramChannel() = 0;
	virtual Shared<DatagramChannel> openDatagramChannel(ProtocolFamily family) = 0;
	virtual Shared<Pipe> openPipe() = 0;
	virtual Shared<AbstractSelector> openSelector() = 0;
	virtual Shared<ServerSocketChannel> openServerSocketChannel() = 0;
	virtual Shared<SocketChannel> openSocketChannel() = 0;
	virtual Shared<Channel> inheritedChannel() { return null; }
};

class Selector : extends Object, implements io::Closeable {
protected:
	Selector() {}

public:
	static Shared<Selector> open();

	virtual boolean isOpen() const = 0;
	virtual Shared<SelectorProvider> provider() = 0;
	//virtual Set<SelectionKey> keys() = 0;
	//Set<SelectionKey> selectedKeys() = 0;
	virtual int selectNow() = 0;
	virtual int select(long timeout) = 0;
	virtual int select() = 0;
	virtual Selector& wakeup() = 0;
	virtual void close() = 0;
};

class AbstractSelector : extends Selector {
private:
	Shared<SelectorProvider> mProvider;
	bool selectorOpen;

protected:
	AbstractSelector(SelectorProvider& provider) : mProvider(&provider) {
	}
public:
	virtual void close() final {
		//boolean open = selectorOpen.getAndSet(false);
		boolean open = selectorOpen;
        if (!open) return;
		//implCloseSelector();
	}
	virtual boolean isOpen() const final {
		//return selectorOpen.get();
		return selectorOpen;
	}
	virtual Shared<SelectorProvider> provider() {
		return mProvider;
	}
	virtual int select() = 0;
	virtual int selectNow() = 0;
	virtual Selector& wakeup() = 0;
};

}}

#endif
