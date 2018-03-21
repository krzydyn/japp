#ifndef __UTIL_LIST_HPP
#define __UTIL_LIST_HPP

#include <lang/String.hpp>

namespace util {

namespace helper {
void throwUnsupportedOperationException();
}

template<class T>
interface Iterator : Interface {
protected:
	Iterator() {}
public:
	virtual bool hasNext() const = 0;
	virtual const T& next() = 0;
	virtual void remove() {TRACE;helper::throwUnsupportedOperationException();}
};

template<class T>
using SharedIterator = Shared<Iterator<T>>;

template<class T>
interface Iterable : Interface {
public:
	Iterable(){}
	virtual SharedIterator<T> iterator() = 0;
};

template<class T>
interface Collection : extends Iterable<T> {
public:
	Collection(){}
	virtual int size() const = 0;
	virtual boolean isEmpty() const final {return size()==0;}
	virtual boolean contains(const T& v) const = 0;
	virtual SharedIterator<T> iterator() = 0;
	virtual Array<T> toArray() const {TRACE;
		Array<T> a(size());
		int ai=0;
		for (SharedIterator<T> i = const_cast<Collection<T>&>(*this).iterator(); i->hasNext(); ++ai) {
			a[ai] = i->next();
		}
		return a;
	}
	virtual boolean add(const T& v) = 0;
	virtual boolean remove(const T& v) = 0;
	virtual boolean containsAll(const Collection<T>& c) const {TRACE;
		for (SharedIterator<T> i = const_cast<Collection<T>&>(c).iterator(); i->hasNext(); ) {
			if (!contains(i->next())) return false;
		}
		return true;
	}
	virtual void addAll(const Collection<T>& c) {TRACE;
		for (SharedIterator<T> i = const_cast<Collection<T>&>(c).iterator(); i->hasNext(); ) {
			add(i->next());
		}
	}
	virtual void removeAll(const Collection<T>& c) {TRACE;
		for (SharedIterator<T> i = iterator(); i->hasNext(); ) {
			if (c.contains(i->next())) i->remove();
		}
	}
	/**
	 * removes from this collection all of its elements that are not contained in the
	 * specified collection.
	 */
	virtual void retainAll(Collection<T>& set) {TRACE;
	}
	virtual void clear() = 0;

};

template<class T>
interface List : extends Collection<T> {
public:
	List(const List& other) = delete;
	List(List&& other) = delete;
	List& operator=(const List& other) = delete;
	List& operator=(List&& other) = delete;
	virtual ~List() {}

	List() : mEnd(this) {}

	virtual boolean contains(const T& v) const final {TRACE; return indexOf(v) < size(); }
	virtual int size() const = 0;
	virtual const T& get(int i) const = 0;
	virtual T& get(int i) = 0;
	virtual void set(int i, const T& v) = 0;
	virtual void add(int i, const T& v) = 0;
	virtual T removeAt(int i) = 0;
	virtual int indexOf(const T& v,int start=0) const = 0;
	virtual int lastIndexOf(const T& v,int start=0) const = 0;
	//virtual List<T>& subList(int fromIndex, int toIndex) {}

	virtual boolean add(const T& v) {TRACE;add(-1,v);return true;}
	virtual boolean remove(const T& v) {TRACE;
		int i = indexOf(v);
		if (i < size()) {removeAt(i);return true;}
		return false;
	}

	virtual boolean addAll(int index, const Collection<T>& c) {TRACE;
		int ai=0;
		for (SharedIterator<T> i = const_cast<Collection<T>&>(c).iterator(); i->hasNext(); ++ai) {
			add(index+ai, i->next());
		}
		return true;
	}

	// generic stack interface (LIFO)
	virtual void push(const T& v) final {TRACE;add(-1,v);}
	virtual T pop() final {TRACE;return removeAt(size()-1);}

	// generic queue interface (FIFO)
	virtual void enqueue(const T& v) final {TRACE;add(-1,v);}
	virtual T dequeue() final {TRACE;return removeAt(0U);}

	// c++11 range-based for loops support
	class ListRange {
	friend class List;
	private:
		List<T>& ref;
		int idx = 0;
		ListRange(List<T>* a) : ref(*a) {}
	public:
		int operator++() { return ++idx; } //preincrement
		//int operator++(int) { return idx++; } //postincrement
		//int operator==(const ListRange& o) { return idx == o.idx; }
		int operator!=(const ListRange& o) const { return idx != o.idx; }
		T& operator*() {return ref.get(idx);}
	};

	ListRange begin() { mEnd.idx=size(); return ListRange(this); }
	const ListRange& end() { return mEnd; }

private:
	ListRange mEnd;
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
	SharedIterator<T> iterator() = 0;

	String toString() const {TRACE;
		StringBuilder s;
		s.append("[");
		SharedIterator<T> i = const_cast<AbstractList<T>&>(*this).iterator();
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
