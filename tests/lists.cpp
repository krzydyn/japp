#include <lang/System.hpp>
#include <util/ArrayList.hpp>
#include <util/LinkedList.hpp>
#include <util/HashMap.hpp>

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
	System.out.println("ArrayList toString = "+list.toString());
	System.out.println("ArrayList simplename = "+list.getClass().getSimpleName());
	System.out.println("guard="+String::valueOf(guard));

	for (int i=1; i < 10; ++i) {list.enqueue(i);++guard;}
	for (Iterator<int> i = list.iterator(); i->hasNext(); ) {
		int v = i->next();
		System.out.println("val = "+String::valueOf(v)+"a");
	}
}

static void test_ArrayList2() {
	ArrayList<String> list;

	list.add("aaa");
	list.add(String("bbb"));
	System.out.println("list.toString");
	System.out.println(list.toString());
}

static void test_LinkedList() {
}

static void test_HashMap() {
	HashMap<int,String> map;
	map.put(1,"a1");
	map.put(2,"b2");
}

int main(int argc, const char *argv[]) {
	System.out.println("ArrayList - simple types");
	test_ArrayList();
	System.out.println("ArrayList - Objects");
	test_ArrayList2();
	System.out.println("LinkedList");
	test_LinkedList();
	System.out.println("test HashMap");
	test_HashMap();
	return 0;
}
