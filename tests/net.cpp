#include <nio/channels/Channel.hpp>

using namespace nio::channels;

void test_datagrams() {
	String addr = "localhost";
	int port = 8000;
	Shared<Selector> selector = Selector::open();
	Shared<DatagramChannel> chn1 = selector->provider()->openDatagramChannel();
	Shared<DatagramChannel> chn2 = selector->provider()->openDatagramChannel();
	if (chn1 == null || chn2 == null) {
		System.out.println("can't open socket channel");
	}
	else {
		Shared<nio::ByteBuffer> src1 = nio::ByteBuffer::allocate(100);
		Shared<nio::ByteBuffer> src2 = nio::ByteBuffer::allocate(200);
		chn1->configureBlocking(false);
		chn2->configureBlocking(false);
		chn1->bind(InetSocketAddress(addr, port));
		chn2->connect(InetSocketAddress(addr, port));

		src1->clear();
		for (int i=0; i < src1->limit(); ++i) src1->put((byte)i);
		src1->flip();
		chn2->write(*src1);

		src2->clear();
		const SocketAddress& cli = chn1->receive(*src2);
		Log.log("received 1");

		src1->clear();
		for (int i=0; i < src1->limit(); ++i) src1->put((byte)i);
		src1->flip();
		chn1->send(*src1, cli);
		src2->clear();
		chn2->receive(*src2);
		Log.log("received 2");
	}
	selector->wakeup();
}

void test_sockets() {
	String addr = "localhost";
	int port = 8000;
	Shared<Selector> selector = Selector::open();
	Shared<SocketChannel> chn1 = selector->provider()->openSocketChannel();
	Shared<SocketChannel> chn2 = selector->provider()->openSocketChannel();
	if (chn1 == null || chn2 == null) {
		System.out.println("can't open socket channel");
	}
	else {
		chn2->connect(InetSocketAddress(addr, port));
		
	}
}

void test_selector() {
}

int main() {
	test_datagrams();
	test_sockets();
}
