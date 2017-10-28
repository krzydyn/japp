#ifndef __UTIL_LIST_HPP
#define __UTIL_LIST_HPP

//#include <lang/System.hpp>
#include <lang/String.hpp>
#include <util/Iterator.hpp>

#define END_OF_LIST ((unsigned)-1)

namespace util {

template<class T>
class Iterable {
public:
	Iterable(const Iterable& other) = delete;
	Iterable(Iterable&& other) = delete;
	Iterable& operator=(const Iterable& other) = delete;
	Iterable& operator=(Iterable&& other) = delete;
	virtual ~Iterable() {}

	Iterable(){}
	virtual Iterator<T> iterator() = 0;
};

template<class T>
class Collection : extends Iterable<T> {
public:
	Collection(const Collection& other) = delete;
	Collection(Collection&& other) = delete;
	Collection& operator=(const Collection& other) = delete;
	Collection& operator=(Collection&& other) = delete;
	virtual ~Collection() {}

	Collection(){}
	virtual unsigned size() const = 0;
	virtual boolean isEmpty() const final {return size()==0;}
	virtual boolean contains(const T& v) const = 0;
	virtual Iterator<T> iterator() = 0;
	virtual Array<T> toArray() const {
		Array<T> a(size());
		/*unsigned ai=0;
		for (Iterator<T> i = iterator(); i->hasNext(); ++ai) {
			a[ai] = i->next();
		}*/
		return a;
	}
	virtual boolean add(const T& v) = 0;
	virtual boolean remove(const T& v) = 0;
	virtual boolean containsAll(const Collection<T>& c) {return false;}
	virtual void addAll(Collection<T>& c) {
		for (Iterator<T> i = c.iterator(); i->hasNext(); ) {
			add(i->next());
		}
	}
	virtual void removeAll(const Collection<T>& c) {
		for (Iterator<T> i = iterator(); i->hasNext(); ) {
			if (c.contains(i->next())) i->remove();
		}
	}
	/**
	 * removes from this collection all of its elements that are not contained in the
	 * specified collection.
	 */
	virtual void retainAll(Collection<T>& set) {}
	virtual void clear() = 0;
};

template<class T>
class List : extends Collection<T> {
public:
	List(const List& other) = delete;
	List(List&& other) = delete;
	List& operator=(const List& other) = delete;
	List& operator=(List&& other) = delete;
	virtual ~List() {}

	List() {}

	virtual boolean contains(const T& v) const final { return indexOf(v) < size(); }
	virtual unsigned size() const = 0;
	virtual const T& get(unsigned i) const = 0;
	virtual T& get(unsigned i) = 0;
	virtual void set(unsigned i, const T& v) = 0;
	virtual void add(unsigned i, const T& v) = 0;
	virtual T removeAt(unsigned i) = 0;
	virtual unsigned indexOf(const T& v,unsigned start=0) const = 0;
	virtual unsigned lastIndexOf(const T& v,unsigned start=0) const = 0;
	//virtual List<T>& subList(int fromIndex, int toIndex) {}

	virtual boolean add(const T& v) {add(END_OF_LIST,v);return true;}
	virtual boolean remove(const T& v) {
		unsigned i = indexOf(v);
		if (i < size()) {removeAt(i);return true;}
		return false;
	}

	virtual boolean addAll(int index, const Collection<T>& c) {return false;}

	// stack interface (LIFO)
	virtual void push(const T& v) final {add(END_OF_LIST,v);}
	virtual T pop() final {return removeAt(size()-1);}

	// queue interface (FIFO)
	virtual void enqueue(const T& v) final {add(END_OF_LIST,v);}
	virtual T dequeue() final {return removeAt(0U);}
};

template<class T>
class AbstractList : extends Object, implements List<T> {
public:
	AbstractList(const AbstractList& other) = delete;
	AbstractList(AbstractList&& other) = delete;
	AbstractList& operator=(const AbstractList& other) = delete;
	AbstractList& operator=(AbstractList&& other) = delete;
	virtual ~AbstractList() {}

	AbstractList() {}
	Iterator<T> iterator() = 0;
	String toString() const {
		StringBuilder s;
		s.append("[");
		Iterator<T> i = const_cast<AbstractList&>(*this).iterator();
		if (i->hasNext()) s.append(i->next());
		while (i->hasNext()) {
			s.append(",");
			s.append(i->next());
		}
		s.append("]");
		return s.toString();
	}
};

} //namespace util

using namespace util;

#endif
