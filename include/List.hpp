#ifndef __LIST_HPP
#define __LIST_HPP

#include <iostream>
#include <types.hpp>

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
	boolean empty() const {return size()==0;}
	virtual const T& ref(unsigned i) const = 0;
	virtual T& ref(unsigned i) = 0;
	virtual T get(unsigned i) const = 0;
	virtual void add(const T v) {insert(size(),v);}
	virtual void insert(unsigned i,const T v) = 0;
	virtual unsigned indexOf(const T v,unsigned start=0) const = 0;

	//TODO implement ListIterator
	virtual void remove(List<T>& set) {
		unsigned d=0;
		for (unsigned i=0; i<size(); ++i) {
			const T& r = ref(i);
			unsigned p;
			if ((p=set.indexOf(r)) != eol) remove(p);
		}
	}
	virtual T remove(unsigned i) = 0;

	// stack interface (LIFO)
	void push(T v) {insert(size(),v);}
	T pop() {return remove(size()-1);}

	// queue interface (FIFO)
	void enqueue(T v) {insert(size(),v);}
	T dequeue() {return remove(0);}

	virtual void print() {
		for (unsigned i=0; i<size(); ++i) std::cout << ref(i);
		std::cout << std::endl;
	}
};

#endif
