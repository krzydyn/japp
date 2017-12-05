#ifndef __UTIL_ARRAYLIST_HPP
#define __UTIL_ARRAYLIST_HPP

#include <lang/Math.hpp>
#include <lang/Exception.hpp>
#include <lang/Class.hpp>
#include <util/List.hpp>
#include <utility> //std::move

namespace util {

inline boolean util_equals(int a, int b) { return a == b; }
inline boolean util_equals(const Object& a, const Object& b) { return a.equals(b); }
template<class T>
inline boolean util_equals(const T& a, const T& b) { return (void*)&a == (void*)&b; }

template<class T>
class ArrayList : extends AbstractList<T> {
class ArrayListIterator;
private:
	T *mVec;
	unsigned mOffs,mSize, mCapa;
	void ensureCapa(unsigned ns){TRACE;
		if (mCapa>=ns) return ;
		if (mCapa == 0) ;
		else if (ns<8) ns=8;
		else ns=hiBit(ns)<<1;
		T *v = new T[ns];
		if (!v) throw OutOfMemoryError();
		if (mVec) {
			for (unsigned i=0; i < mSize; ++i) v[i]=std::move(mVec[(mOffs+i)%mCapa]);
			delete [] mVec;
		}
		mOffs=0;
		mVec=v; mCapa=ns;
	}

public:
	ArrayList(int initCapa=0) {TRACE;
		mVec=null;mOffs=mSize=0;mCapa=0;
		if (initCapa) ensureCapa((unsigned)initCapa);
	}
	~ArrayList() {TRACE; delete [] mVec; }

	SharedIterator<T> iterator() {TRACE;
		return makeShared<ArrayListIterator>(*this);
	}
	void clear() { mOffs=mSize=0; }
	int size() const {return (int)mSize;}
	void shrink(int s) {if (s < mSize) mSize=s;}
	const T& get(int i) const {TRACE;
		unsigned j=(unsigned)i;
		if (j >= mSize) throw IndexOutOfBoundsException(i);
		return mVec[(mOffs+j)%mCapa];
	}
	T& get(int i) {TRACE;
		unsigned j=(unsigned)i;
		if (j >= mSize) throw IndexOutOfBoundsException(i);
		return mVec[(mOffs+j)%mCapa];
	}
	void set(int i, const T& v) {TRACE;
		unsigned j=(unsigned)i;
		if (j >= mSize) throw IndexOutOfBoundsException(i);
		mVec[(mOffs+j)%mCapa] = v;
	}

	//in C++ methods of the same name from base class are hidden by default
	// - so tell explicitly not to hide by "using"
	using List<T>::add;
	using List<T>::remove;

	void add(int i,const T& v) {TRACE;
		if (mSize >= mCapa) ensureCapa(mSize+1);
		if (i == -1) {
			mVec[(mOffs+mSize)%mCapa]=v;
			++mSize;
		   	return ;
	   	}
		unsigned j=(unsigned)i;
		if (j > mSize) throw IndexOutOfBoundsException(i);
		unsigned ii;
		if (2*j < mSize) {
			mOffs = (mOffs + mCapa - 1)%mCapa;
			for (ii=0; ii < j; ++ii) mVec[(mOffs+ii)%mCapa]=mVec[(mOffs+ii+1)%mCapa];
		}
		else {
			for (ii=mSize; ii > j; --ii) mVec[(mOffs+ii)%mCapa]=mVec[(mOffs+ii-1)%mCapa];
		}
		mVec[(mOffs+j)%mCapa]=v; ++mSize;
	}
	int indexOf(const T& v,int start=0) const {TRACE;
		for (unsigned i=(unsigned)start; i < mSize; ++i ) {
			if (util_equals(mVec[(mOffs+i)%mCapa],v)) return (int)i;
		}
		return -1;
	}
	int lastIndexOf(const T& v,int start=0) const {TRACE;
		return -1;
	}
	void removeAll(const Collection<T>& c) {TRACE;
		unsigned d=0;
		for (unsigned i=0; i<mSize; ++i) {
			//copy elements not existing on c
			if (!c.contains(mVec[(mOffs+i)%mCapa])) {
				if (d!=i) mVec[(mOffs+d)%mCapa]=mVec[(mOffs+i)%mCapa];
				++d;
			}
		}
		mSize=d;
	}
	T removeAt(int i) {TRACE;
		if (i==-1) throw IndexOutOfBoundsException(i);
		unsigned j = (unsigned)i;
		if (j >= mSize) throw IndexOutOfBoundsException(i);
		T v=mVec[(mOffs+j)%mCapa]; --mSize;
		if (j < mSize - j) {
			for (; j > 0; --j) {
				mVec[(mOffs+j)%mCapa]=mVec[(mOffs+j-1)%mCapa];
			}
			++mOffs;
		}
		else {
			for (; j < mSize; ++j) mVec[(mOffs+j)%mCapa]=mVec[(mOffs+j+1)%mCapa];
		}
		return v;
	}

	void comboSort() {TRACE;
		int gap = mSize;
		boolean swapped=false;
		while (gap > 1 || swapped) {
			gap = gap * 10 / 13; //empirical
			if (gap==0) gap=1;
			else if (gap==9||gap==10) gap=11;

			swapped = false;
			for (int i = 0; i + gap < mSize; ++i) {
				T x=mVec[i+gap];
				if (x < mVec[i]) {
					mVec[i+gap]=mVec[i];
					mVec[i]=x;
					swapped = true;
				   }
			  }
		}
	}
	void isort() {TRACE;
		for (unsigned i=0; i<mSize; ++i) {
			T e=mVec[i];
			unsigned iv=i;
			for (unsigned j=i+1; j<mSize; ++j) {
				T x=mVec[j];
				if (x < e) { e=x; iv=j; }
			}
			if (iv!=i) { e=mVec[i]; mVec[i]=mVec[iv]; mVec[iv]=e; }
		}
	}
	void qsort(int l, int h) {TRACE;
		T e = mVec[l+(h-l)/2]; //pivot element
		int i=l,j=h;
		do {
			while (mVec[i] < e) ++i;
			while (e < mVec[j]) --j;
			if (i <= j) {
				if (i!=j) {T x=mVec[i]; mVec[i]=mVec[j]; mVec[j]=x;}
				++i; --j;
			}
		} while (i<=j);
		if (l < j) qsort(l, j);
		if (i < h) qsort(i, h);
	}

private:
	class ArrayListIterator : extends Iterator<T> {
	private:
		ArrayList<T>& mList;
		unsigned mNext;

	public:
		ArrayListIterator(ArrayList& list) : mList(list), mNext(0) {}
		bool hasNext() const {TRACE; return mNext < mList.mSize; }
		const T& next() {TRACE; ++mNext; return mList.get((int)(mNext-1)); }
		void remove() {TRACE;
			if (mNext > 0) {
				--mNext;
				mList.removeAt((int)mNext);
			}
		}
	};
};

} //namespace util

#endif
