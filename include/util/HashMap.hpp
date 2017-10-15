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
private:
    K key;
    V val;
};

template<class K,class V>
class Map {
public:
    virtual ~Map() {}
    virtual unsigned size() = 0;
	virtual boolean isEmpty() = 0;
	virtual boolean containsKey(const K& key) = 0;
	virtual boolean containsValue(const V& value) = 0;
};

unsigned hashCode(unsigned x) {return x;}

template<class K,class V>
class HashMap {
public:
    HashMap() {init(10);}
    HashMap(unsigned s) {init(s);}
    ~HashMap() { if (map) delete []map; }

    unsigned size() {return elems;}
	boolean isEmpty() { return elems == 0; }

	boolean containsKey(const K& key) {
		return false;
	}
	boolean containsValue(const V& value) {
		return false;
	}

	V& get(const K& k) const {
		V v;
		if (get(k,v)) return v;
		return *((V*)null);
	}
    bool get(const K& k, V& v) const {
        unsigned hc=hashCode(k)%mapsize;
        const ArrayList<MapEntry<K,V> >& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                v=l.ref(i).getVal();
                return true;
            }
        }
        return false;
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
        l.push(me); ++elems;
    }
    bool remove(const K& k, const V& v) {
        unsigned hc=hashCode(k)%mapsize;
        const ArrayList<MapEntry<K,V>>& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                v=l.remove(i).getVal(); --elems;
                return true;
            }
        }
        return false;
    }
    const MapEntry<K,V>& entry(unsigned i) {
        for (int hc=0; hc<mapsize; ++hc) {
            ArrayList<MapEntry<K,V>>& l=map[hc];
            if (i < l.size()) { return l.ref(i); }
            i-=l.size();
        }
        //throw exc.
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
