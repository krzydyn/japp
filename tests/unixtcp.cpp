#include <SyncQueue.hpp>
#include <ArrayList.hpp>

namespace std {
	template< class CharT, class Traits >
	std::basic_ostream<CharT, Traits>& nl( std::basic_ostream<CharT, Traits>& os ){
		return endl(os);
	}
}


int main(int argc, const char *argv[]) {
	ArrayList<int> xx;
	SyncQueue<int> sq;
	for (int i=1; i < 10; ++i) sq.enqueue(i);
	while (!sq.empty()) {
		std::cout << sq.dequeue() << std::nl;
	}
	std::cout << "adding.." << std::nl;
	for (int i=1; i < 10; ++i) xx.enqueue(i);
	xx.remove(1);
	std::cout << "printing.." << std::nl;
	while (!xx.empty()) {
		std::cout << xx.dequeue() << std::nl;
	}
	return 0;
}
