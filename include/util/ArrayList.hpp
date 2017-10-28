#ifndef __UTIL_ARRAYLIST_HPP
#define __UTIL_ARRAYLIST_HPP

#include <lang/Math.hpp>
#include <lang/Exception.hpp>
#include <util/List.hpp>
#include <iostream>

namespace util {

inline boolean util_equals(int a, int b) { return a == b; }
inline boolean util_equals(const Object& a, const Object& b) { return a.equals(b); }
template<class T>
inline boolean util_equals(const T& a, const T& b) { return (void*)&a == (void*)&b; }

template<class T>
class ArrayList : public AbstractList<T> {
class ArrayListIterator;

public:
    ArrayList(unsigned initCapa=0) {
        mVec=null;mOffs=mSize=0;mCapa=0;
        if (initCapa) ensureCapa(initCapa);
    } 
    ~ArrayList() { delete [] mVec; } 

	Iterator<T> iterator() {
		return makeIterator<ArrayListIterator>(*this);
	}
    void clear() { mOffs=mSize=0; }
    unsigned size() const {return mSize;}
    void shrink(unsigned s) {if (s < mSize) mSize=s;}
    const T& get(unsigned i) const {
		if (i >= mSize) throw IndexOutOfBoundsException(i);
		return mVec[(mOffs+i)%mCapa];
	}
    T& get(unsigned i) {
		if (i >= mSize) throw IndexOutOfBoundsException(i);
		return mVec[(mOffs+i)%mCapa];
	}
	void set(unsigned i, const T& v) {
		if (i >= mSize) throw IndexOutOfBoundsException(i);
		mVec[(mOffs+i)%mCapa] = v;
	}

	//in C++ methods of the same name from base class are hidden by default
	// - so tell explicitly not to hide by "using"
	using List<T>::add;
	using List<T>::remove;

    void add(unsigned i,const T& v) {
        if (mSize>=mCapa) ensureCapa(mSize+1);
        if (i == END_OF_LIST) {
			printf("copy %s into mvec[%d]\n", Class::nameOf(v).c_str(), (mOffs+mSize)%mCapa);
			mVec[(mOffs+mSize)%mCapa]=v;
			//memcpy(mVec + ((mOffs+mSize)%mCapa), &v, sizeof(v));
			++mSize;
		   	return ;
	   	}
		if (i > mSize) throw IndexOutOfBoundsException(i);
        unsigned j=i;
		if (i < mSize - i) {
			mOffs = (mOffs + mCapa - 1)%mCapa;
			for (i=0; i < j; ++i) mVec[(mOffs+i)%mCapa]=mVec[(mOffs+i+1)%mCapa];
		}
		else {
			for (i=mSize; i > j; --i) mVec[(mOffs+i)%mCapa]=mVec[(mOffs+i-1)%mCapa];
		}
		printf("copy into mvec\n");
        mVec[(mOffs+j)%mCapa]=v; ++mSize;
    }
    unsigned indexOf(const T& v,unsigned start=0) const {
		for (unsigned i=start; i < mSize; ++i ) {
			if (util_equals(mVec[(mOffs+i)%mCapa],v)) return i;
		}
		return END_OF_LIST;
	}
    unsigned lastIndexOf(const T& v,unsigned start=0) const {
		return END_OF_LIST;
	}
    void removeAll(const Collection<T>& c) {
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
    T removeAt(unsigned i) {
		if (i >= mSize) throw IndexOutOfBoundsException(i);
        T v=mVec[(mOffs+i)%mCapa]; --mSize;
		if (i < mSize - i) {
        	for (; i > 0; --i) {
				mVec[(mOffs+i)%mCapa]=mVec[(mOffs+i-1)%mCapa];
			}
			++mOffs;
		}
		else {
        	for (; i < mSize; ++i) mVec[(mOffs+i)%mCapa]=mVec[(mOffs+i+1)%mCapa];
		}
        return v;
    }

	void comboSort() {
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
    void isort() { //simple insert sort
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
    void qsort(int l, int h) { //quick sort
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
    T *mVec;
    unsigned mOffs,mSize, mCapa;
    void ensureCapa(unsigned ns){
		if (mCapa>=ns) return ;
		if (ns<8) ns=8;
		else ns=hiBit(ns)<<1;
		T *v = new T[ns];
		if (!v) throw OutOfMemoryError();
		if (mVec) {
			for (unsigned i=0; i < mSize; ++i) v[i]=std::move(mVec[(mOffs+i)%mCapa]);
			delete [] mVec;
		}
		mOffs=0;
		mVec=v; mCapa=ns;
		//std::cerr << "capa = " << mCapa << std::endl;
	}

	class ArrayListIterator : public IteratorBase<T> {
	private:
		ArrayList<T>& mList;
    	unsigned mNext;

	public:
		ArrayListIterator(ArrayList&list) : mList(list), mNext(0) {}
		bool hasNext() { return mNext < mList.mSize; }
		const T& next() { return mList.get(mNext++); }
		void remove() {
			if (mNext > 0) {
				--mNext;
				mList.removeAt(mNext);
			}
		}
	};
};

} //namespace util

#endif
