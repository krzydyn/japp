#ifndef __UTIL_LINKEDLIST_HPP
#define __UTIL_LINKEDLIST_HPP

#include <util/List.hpp>

namespace util {

template<class T>
class LinkedListNode {
	LinkedListNode<T> *mNext, *mPrev;
public:
	T item;

	LinkedListNode() {mPrev=mNext=this;}
	LinkedListNode(T e) {item=e;mPrev=mNext=this;}
	~LinkedListNode() { remove(); }

	void insertAfter(LinkedListNode<T> *p) {
		mPrev = p;
		mNext = p->mNext;
		mPrev->mNext = this;
		mNext->mPrev = this;
	}
	void remove() {
		if (mPrev) mPrev->mNext = mNext;
		if (mNext) mNext->mPrev = mPrev;
		mNext = mPrev = null;
	}
	LinkedListNode<T> *next() const { return mNext; }
	LinkedListNode<T> *prev() const { return mPrev; }
};

template<class T>
class LinkedList : public AbstractList<T> {
	LinkedListNode<T> head;
	unsigned mSize;
public:
	LinkedList() { mSize=0; }
	~LinkedList() {
		while (head.next()!=&head) removeLast();
		if (mSize) fprintf(stderr,"size=%d, no nodes on list\n",mSize);
	}
	unsigned size() {return mSize;}
	void append(const T& v) {
		LinkedListNode<T> *n=new LinkedListNode<T>(v);
		n->insertAfter(head.prev());
		++mSize;
	}
	void prepend(const T& v) {
		LinkedListNode<T> *n=new LinkedListNode<T>(v);
		n->insertAfter(&head);
		++mSize;
	}
	void removeAll(const T& v) {
		for (LinkedListNode<T> *n=head.next(); n != &head; ) {
			LinkedListNode<T> *next = n->next();
			if (n->item == v) {--mSize;n->remove();}
			n=next;
		}
	}
	void addFirst(const T& v) {prepend(v);}
	void addLast(const T& v) {append(v);}
	LinkedListNode<T> *first() { LinkedListNode<T> *n=head.next(); return n==&head ? null : n;}
	LinkedListNode<T> *last() { LinkedListNode<T> *n=head.prev(); return n==&head ? null : n;}
	T removeFirst() {
		if (head.next()==&head) return head.item;
		LinkedListNode<T> *e = head.next();
		--mSize;
		e->remove();
		T v = e->item;
		delete e;
		return v;
	}
	T removeLast() {
		if (head.next()==&head) return head.item;
		LinkedListNode<T> *e = head.prev();
		--mSize;
		e->remove();
		T v = e->item;
		delete e;
		return v;
	}
};

} //namespace util

#endif
