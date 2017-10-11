#ifndef __LIST_HPP
#define __LIST_HPP

#include <iostream>
#include <lang/Types.hpp>
#include <Iterator.hpp>

template<class T>
class List {
public:
	List(const List& other) = delete;
	List(List&& other) = delete;
	List& operator=(const List& other) = delete;
	List& operator=(List&& other) = delete;

	List() {}
	virtual ~List() {}
	virtual void clear() = 0;

	virtual unsigned size() const = 0;
	boolean isEmpty() const {return size()==0;}
	boolean contains(const T& v) { return indexOf(v) < size(); }

	virtual std::shared_ptr<Iterator<T>> iterator() = 0;

	virtual unsigned indexOf(const T& v,unsigned start=0) const = 0;
	virtual T copyOf(unsigned i) const = 0;
	virtual const T& get(unsigned i) const = 0;
	virtual T& ref(unsigned i) const = 0;
	virtual void set(unsigned i, const T v) = 0;

	virtual boolean add(const T& v) {add(size(),v);return true;}
	virtual void add(unsigned i, const T v) = 0;
	virtual boolean removeElem(const T& v) {
		unsigned i = indexOf(v);
		if (i < size()) {remove(indexOf(v));return true;}
		return false;
	}
	virtual T remove(unsigned i) = 0;

	//TODO implement ListIterator
	virtual void removeAll(List<T>& set) {
		unsigned d=0;
		for (unsigned i=0; i<size(); ++i) {
			const T& r = get(i);
			unsigned p;
			if ((p=set.indexOf(r)) != eol) remove(p);
		}
	}

	// stack interface (LIFO)
	void push(T v) {add(size(),v);}
	T pop() {return remove(size()-1);}

	// queue interface (FIFO)
	void enqueue(T v) {add(size(),v);}
	T dequeue() {return remove(0U);}

	virtual void print() {
		for (unsigned i=0; i<size(); ++i) std::cout << get(i);
		std::cout << std::endl;
	}
};

#endif
