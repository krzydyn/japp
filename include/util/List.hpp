#ifndef __UTIL_LIST_HPP
#define __UTIL_LIST_HPP

#include <lang/System.hpp>
#include <util/Iterator.hpp>

#define END_OF_LIST ((unsigned)-1)

namespace util {

template<class T>
class List {
public:
	List(const List& other) = delete;
	List(List&& other) = delete;
	List& operator=(const List& other) = delete;
	List& operator=(List&& other) = delete;
	virtual ~List() = default;

	List() {}
	virtual void clear() = 0;

	virtual unsigned size() const = 0;
	virtual boolean isEmpty() const final {return size()==0;}
	virtual boolean contains(const T& v) const final { return indexOf(v) < size(); }

	virtual Iterator<T> iterator() = 0;

	virtual unsigned indexOf(const T& v,unsigned start=0) const = 0;
	virtual T copyOf(unsigned i) const = 0;
	virtual const T& get(unsigned i) const = 0;
	virtual T& ref(unsigned i) const = 0;
	virtual void set(unsigned i, const T& v) = 0;

	virtual boolean add(const T& v) {add(END_OF_LIST,v);return true;}
	virtual void add(unsigned i, const T& v) = 0;
	virtual boolean removeElem(const T& v) {
		unsigned i = indexOf(v);
		if (i < size()) {remove(indexOf(v));return true;}
		return false;
	}
	virtual T remove(unsigned i) = 0;

	virtual void removeAll(List<T>& set) {
		unsigned d=0;
		for (Iterator<T> i = iterator(); i->hasNext(); ) {
			unsigned p;
			if ((p=set.indexOf(i->next())) != END_OF_LIST) i->remove();
		}
	}

	// stack interface (LIFO)
	virtual void push(const T& v) final {add(END_OF_LIST,v);}
	virtual T pop() final {return remove(size()-1);}

	// queue interface (FIFO)
	virtual void enqueue(const T& v) final {add(END_OF_LIST,v);}
	virtual T dequeue() final {return remove(0U);}

	virtual void print() {
		System.out.print("[");
		Iterator<T> i = iterator();
		if (i->hasNext()) System.out.print(i->next());
		while (i->hasNext()) {
			System.out.print(",");
			System.out.print(i->next());
		}
		System.out.println("]");
	}
};

} //namespace util

using namespace util;

#endif
