#include <nio/channels/Channel.hpp>

using namespace nio::channels;
int main() {
	String addr = "localhost";
	int port = 80;
	Shared<Selector> selector = Selector::open();
	Shared<DatagramChannel> chn = selector->provider()->openDatagramChannel();
	if (chn == null) {
		System.out.println("can't open socket channel");
	}
	else {
		chn->configureBlocking(false);
		chn->connect(InetSocketAddress(addr, port));
		chn->disconnect();
		chn->bind(InetSocketAddress(addr, port));
	}
	selector->wakeup();
}
