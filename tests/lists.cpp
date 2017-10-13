#include <util/ArrayList.hpp>

#include <SyncQueue.hpp>

namespace std {
	template< class CharT, class Traits >
	std::basic_ostream<CharT, Traits>& nl( std::basic_ostream<CharT, Traits>& os ){
		return endl(os);
	}
}


static void test_SyncQueue() {
	SyncQueue<int> sq;
	for (int i=1; i < 10; ++i) sq.enqueue(i);
	while (!sq.isEmpty()) {
		std::cout << sq.dequeue() << std::nl;
	}
	std::cout << "adding.." << std::nl;
}

static void test_ArrayList() {
	ArrayList<int> list;
	int guard = 1;
	for (int i=1; i < 10; ++i) {list.enqueue(i);++guard;}
	list.remove(1); --guard;
	std::cout << "printing.." << std::nl;
	while (!list.isEmpty() && guard > 0) {
		std::cout << list.dequeue() << std::nl;
		--guard;
	}
	std::cout << "guard = " << guard << std::nl;

	for (int i=1; i < 10; ++i) {list.enqueue(i);++guard;}
	for (Iterator<int> i = list.iterator(); i->hasNext(); ) {
		int v = i->next();
		std::cout << "val = " << v << std::nl;
	}
}


int main(int argc, const char *argv[]) {
	test_ArrayList();
	return 0;
}
