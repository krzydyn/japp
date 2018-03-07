#include <nio/channels/Channel.hpp>

using namespace nio::channels;
int main() {
	String addr = "localhost";
	int port = 8000;
	Shared<Selector> selector = Selector::open();
	Shared<DatagramChannel> chn = selector->provider()->openDatagramChannel();
	if (chn == null) {
		System.out.println("can't open socket channel");
	}
	else {
		chn->configureBlocking(false);
		chn->bind(InetSocketAddress(addr, port+1));
		chn->connect(InetSocketAddress(addr, port));
		Shared<nio::ByteBuffer> src = nio::ByteBuffer::allocate(100);
		chn->write(*src);
		//chn->disconnect();
		src->flip();
		//chn->bind(InetSocketAddress(addr, port));
		chn->write(*src);
		src->flip();
		chn->receive(*src);
	}
	selector->wakeup();
}
