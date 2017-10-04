#ifndef __ARRAYLIST_HPP
#define __ARRAYLIST_HPP

#include <List.hpp>
#include <Math.hpp>

#define boolean bool
#define null 0
#define eol ((unsigned)-1)

template<class T>
class ArrayList : public List<T> {
public:
    ArrayList(unsigned initCapa=0) {
        mVec=null;mOffs=mSize=0;mCapa=0;
        if (initCapa) ensureCapa(initCapa);
    } 
    ~ArrayList() { if (mVec) delete [] (unsigned char*)mVec; } 
    void clear() { mOffs=mSize=0; }
    unsigned size() const {return mSize;}
    void shrink(unsigned s) {if (s < mSize) mSize=s;}
    const T& ref(unsigned i) const {
		if (i >= mSize) throw std::runtime_error("index out fo range");
		return mVec[(mOffs+i)%mCapa];
	}
    T& ref(unsigned i) {
		if (i >= mSize) throw std::runtime_error("index out fo range");
		return mVec[(mOffs+i)%mCapa];
	}
    T get(unsigned i) const {
		if (i >= mSize) throw std::runtime_error("index out fo range");
		return mVec[(mOffs+i)%mCapa];
	}
    void insert(unsigned i,const T v) {
        if (mSize>=mCapa) ensureCapa(mSize+1);
        if (i == eol) { mVec[(mOffs+mSize)%mCapa]=v; ++mSize; return ; }
		if (i > mSize) throw std::runtime_error("index out fo range");
        unsigned j=i;
        for (i=mSize; i > j; --i) mVec[(mOffs+i)%mCapa]=mVec[(mOffs+i-1)%mCapa];
        mVec[(mOffs+j)%mCapa]=v; ++mSize;
    }
    unsigned indexOf(const T v,unsigned start=0) const {
        for (unsigned i=start; i<mSize; ++i ) { if (mVec[(mOffs+i)%mCapa]==v) return i; }
        return eol;
    }
    void remove(List<T>& set) {
        unsigned d=0;
        for (unsigned i=0; i<mSize; ++i) {
            if (set.indexOf(mVec[(mOffs+i)%mCapa])==eol) {
                if (d!=i) mVec[(mOffs+d)%mCapa]=mVec[(mOffs+i)%mCapa];
                ++d;
            }
        }
        mSize=d;
    }
    T remove(unsigned i) {
        T v;
		if (i >= mSize) throw std::runtime_error("index out fo range");
        v=mVec[(mOffs+i)%mCapa]; --mSize;
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
    void ensureCapa(unsigned ns) {
        if (mCapa>=ns) return ;
        if (ns<8) ns=8;
        else ns=hiBit(ns)<<1;
		std::cerr << "ensure capa = " << ns << std::endl;
        T *v = (T*)new unsigned char[ns*sizeof(T)];
		if (!v) {
        	throw std::runtime_error("Out of memory");
		}
        if (mVec) {
            for (int i=0; i < mSize; ++i) v[i]=mVec[(mOffs+i)%mCapa];
            delete [] (unsigned char*)mVec;
        }
		mOffs=0;
        mVec=v; mCapa=ns;
    }
};

#endif
