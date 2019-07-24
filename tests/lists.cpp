#include <lang/System.hpp>
#include <util/ArrayList.hpp>
#include <util/LinkedList.hpp>
#include <util/HashMap.hpp>

static void test_Array() {
	Array<int> a(5);
	Object& o = a;
	System::out.println("isArray = " + String::valueOf(o.getClass().isArray()));

	int x[5];
	System::out.println("basic array type = " + Object::getClass(typeid(x)).getName());
}
static void test_ArrayList() {
	ArrayList<int> list;
	for (int i=1; i < 10; ++i) list.enqueue(i);
	list.remove(1);
	System::out.println("ArrayList toString = " + list.toString());
	System::out.println("ArrayList class name = " + list.getClass().getSimpleName());
	System::out.println("ArrayList element class name = " + class(list.get(0)).getName());
}

static void test_ArrayList2() {
	ArrayList<String> list;
	list.add("aaa");
	list.add(String("bbb"));
	list.add(String::valueOf(10.0));
	System::out.println("list.toString = " + list.toString());
}

static void test_ArrayListRangeLoop() {
	ArrayList<String> list;
	list.add("aaa");
	list.add(String("bbb"));
	list.add(String::valueOf(10.0));
	System::out.println("range loop on string");
	for (const String& s : list) {
		System::out.println(s);
	}
}

static void test_LinkedList() {
}

static void test_HashMap() {
	HashMap<int,String> map;
	map.put(1,"a1");
	map.put(2,"b2");
	System::out.println("map.toString = " + map.toString());
}

int main(int argc, const char *argv[]) {
	System::out.println("Array");
	test_Array();
	System::out.println("ArrayList - simple types");
	test_ArrayList();
	System::out.println("ArrayList - Objects");
	test_ArrayList2();
	System::out.println("LinkedList");
	test_LinkedList();
	System::out.println("test HashMap");
	test_HashMap();
	System::out.println("test RangeLoop");
	test_ArrayListRangeLoop();
	return 0;
}
