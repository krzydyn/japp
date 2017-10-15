#include <util/ArrayList.hpp>

static void test_ArrayList() {
	ArrayList<int> list;
	int guard = 1;
	for (int i=1; i < 10; ++i) {list.enqueue(i);++guard;}
	list.remove(1); --guard;
	System.out.println("printing...");
	while (!list.isEmpty() && guard > 0) {
		System.out.println(list.dequeue());
		--guard;
	}
	System.out.println(String("ArrayList toString = ")+list.toString());
	System.out.println(String("ArrayList simplename = ")+list.getClass().getSimpleName());
	System.out.println(String("guard=")+guard);

	for (int i=1; i < 10; ++i) {list.enqueue(i);++guard;}
	for (Iterator<int> i = list.iterator(); i->hasNext(); ) {
		int v = i->next();
		System.out.println(String("val = ")+v+"a");
	}
}

static void test_ArrayList2() {
	ArrayList<String> list;
	list.add(String("aaa"));
	System.out.println("list.print");
	list.print();
}
/*
#include <util/HashMap.hpp>
static void test_Map() {
	HashMap<int,String> map;
	map.put(1,"a");
}
*/
int main(int argc, const char *argv[]) {
	System.out.println("test1");
	test_ArrayList();
	System.out.println("test2");
	test_ArrayList2();
	//test_Map();
	return 0;
}
