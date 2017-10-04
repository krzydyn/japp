#ifndef __LINKLIST_HPP
#define __LINKLIST_HPP

template<class T>
class DListNode {
	DListNode<T> *mNext, *mPrev;
public:
	T item; 

	DListNode() {mPrev=mNext=this;}
	DListNode(T e) {item=e;mPrev=mNext=this;}
	~DListNode() { remove(); }

	void insertAfter(DListNode<T> *p) {
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
	DListNode<T> *next() const { return mNext; }
	DListNode<T> *prev() const { return mPrev; }
};
template<class T>
class DList {
	DListNode<T> head;
	unsigned mSize;
public:
	DList() { mSize=0; }
	~DList() {
		while (head.next()!=&head) removeLast();
		if (mSize) fprintf(stderr,"size=%d, no nodes on list\n",mSize);
	}
	unsigned size() {return mSize;}
	void append(T v) {
		DListNode<T> *n=new DListNode<T>(v);
		n->insertAfter(head.prev());
		++mSize;
	}
	void prepend(T v) {
		DListNode<T> *n=new DListNode<T>(v);
		n->insertAfter(&head);
		++mSize;
	}
	void removeAll(T v) {
		for (DListNode<T> *n=head.next(); n != &head; ) {
			DListNode<T> *next = n->next();
			if (n->item == v) {mSize;n->remove();}
			n=next;
		}
	}
	DListNode<T> *first() { DListNode<T> *n=head.next(); return n==&head ? null : n;}
	DListNode<T> *last() { DListNode<T> *n=head.prev(); return n==&head ? null : n;}
	T removeFirst() {
		if (head.next()==&head) return head.item;
		DListNode<T> *e = head.next();
		--mSize;
		e->remove();
		T v = e->item;
		delete e;
		return v;
	}
	T removeLast() {
		if (head.next()==&head) return head.item;
		DListNode<T> *e = head.prev();
		--mSize;
		e->remove();
		T v = e->item;
		delete e;
		return v;
	}
};

#endif
