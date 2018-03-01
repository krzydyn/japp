#include <nio/channels/Channel.hpp>

int main() {
	Shared<nio::channels::Selector> selector = nio::channels::Selector::open();
}
