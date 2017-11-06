#include<iostream>
#include<vector>
#include<type_traits>

using namespace std;
class Base {
public:
	virtual void print() const {cout << "B" << endl;}
};
class Derived : public Base {
	int x;
public:
	Derived(int _x) : x(_x) {}
	virtual void print() const {cout << "D" << x << endl;}
};

inline void print_elem(const Base& e) { e.print(); }
template<class T, class enable_if<!is_base_of<Base,T>::value,Base>::type* = nullptr>
//template<class T, class enable_if<!is_base_of<Base,T>::value,Base>::type = std::true_type>
inline void print_elem(const T& e) { cout << e << endl; }

template<class T>
class Collection {
private:
	vector<T> elems;
public:
	void add(const T& v) { elems.push_back(v); }
	void print() {
		for (unsigned i=0; i < elems.size(); ++i)
			print_elem(elems[i]);
	}
};

int main(int argc, const char *argv[]) {
	Collection<int> c1;
	c1.add(2); c1.add(5);
	c1.print();

	Collection<Derived> c2;
	c2.add(Derived(2)); c2.add(Derived(5));
	c2.print();
}
