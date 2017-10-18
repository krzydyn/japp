#ifndef __UTIL_MAP_HPP
#define __UTIL_MAP_HPP

#include <util/ArrayList.hpp>

namespace util {

template<class K,class V>
class MapEntry {
public:
    MapEntry(const MapEntry &me) : key(me.key), val(me.val) {}
    MapEntry(const K &key, const V &val) : key(key), val(val) {}
    const K& getKey() const {return key;}
    const V& getVal() const {return val;}
    void setVal(const V& v) {val=v;}
    V& getRef() {return val;}
private:
    K key;
    V val;
};

template<class K,class V>
class Map {
public:
    virtual ~Map() {}
    virtual unsigned size() const = 0;
	virtual boolean isEmpty() const = 0;
	virtual boolean containsKey(const K& key) const = 0;
	virtual boolean containsValue(const V& value) const = 0;
	virtual const V& get(const K& key) const = 0;
	virtual void put(const K& key, const V& value) = 0;
	virtual V& ref(const K& key) = 0;
	virtual V remove(const K& key) = 0;
	virtual void clear() = 0;
};

long hashCode(long x) {return x;}
long hashCode(const Object& x) {return x.hashCode();}
template<class T>
long hashCode(const T& x) {return (long)(&x);}

template<class K,class V>
class HashMap : public Map<K,V> {
private:
	V undef;
public:
    HashMap() {init(10);}
    HashMap(unsigned s) {init(s);}
    ~HashMap() { if (map) delete []map; }

    unsigned size() const {return elems;}
	boolean isEmpty() const { return elems == 0; }
	boolean containsKey(const K& key) const {
		return false;
	}
	boolean containsValue(const V& value) const {
		return false;
	}

    const V& get(const K& k) const {
        unsigned hc=hashCode(k)%mapsize;
        const ArrayList<MapEntry<K,V> >& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                return l.ref(i).getVal();
            }
        }
        return undef;
    }

    void put(const K& k, const V& v) {
        unsigned hc=hashCode(k)%mapsize;
        ArrayList<MapEntry<K,V>>& l=map[hc];
        //printf("put %u %u hc=%u sz[hc]=%u\n",k,v,hc,l.size());
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                l.ref(i).setVal(v);
                return ;
            }
        }
		const MapEntry<K,V> me(k,v);
        l.add(me); ++elems;
    }
	V& ref(const K& k) {
        unsigned hc=hashCode(k)%mapsize;
        const ArrayList<MapEntry<K,V>>& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                return l.ref(i).getRef();
            }
        }
        return undef;
	}
    V remove(const K& k) {
        unsigned hc=hashCode(k)%mapsize;
        ArrayList<MapEntry<K,V>>& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
				--elems;
				return l.remove(i).getVal();
            }
        }
        return undef;
    }
    const MapEntry<K,V>& entry(unsigned i) const {
        for (int hc=0; hc<mapsize; ++hc) {
            ArrayList<MapEntry<K,V>>& l=map[hc];
            if (i < l.size()) { return l.ref(i); }
            i-=l.size();
        }
        //throw exc.
    }
	void clear() {
	}
private:
    void init(unsigned s) {
        map=new ArrayList<MapEntry<K,V> >[s];
        mapsize=s;
    }
    void rehash(unsigned ns) {
    }
    ArrayList<MapEntry<K,V>> *map;
    unsigned mapsize;
    unsigned elems;
};

} // namespace util

#endif
