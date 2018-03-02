#include <nio/channels/Channel.hpp>

using namespace nio::channels;
int main() {
	String addr = "localhost";
	int port = 80;
	Shared<Selector> selector = Selector::open();
	Shared<SocketChannel> chn = selector->provider()->openSocketChannel();
	chn->configureBlocking(false);
	chn->connect(InetSocketAddress(addr, port));
	selector->wakeup();
}
