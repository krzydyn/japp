#ifndef __UTIL_MAP_HPP
#define __UTIL_MAP_HPP

#include <util/ArrayList.hpp>

namespace util {

template<class K,class V>
class MapEntry {
public:
	MapEntry(MapEntry&& o) { key = std::move(o.key); value = std::move(o.value); }
    MapEntry(const MapEntry &me) : key(me.key), value(me.value) {}
	MapEntry& operator=(MapEntry&& o) { key = std::move(o.key); value = std::move(o.value); }
	MapEntry& operator=(const MapEntry& o) { key = o.key; value = o.value; }

    MapEntry() {}
    MapEntry(const K &key, const V &val) : key(key), value(value) {}
    const K& getKey() const {return key;}
    const V& getValue() const {return value;}
    void setVal(const V& v) {value=v;}
    V& getRef() {return value;}
	String toString() { return key + "=" + value; }
private:
    K key;
    V value;
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
	virtual V& get(const K& key) = 0;
	virtual void put(const K& key, const V& value) = 0;
	virtual V remove(const K& key) = 0;
	virtual void clear() = 0;
};

inline long hashCode(long x) {return x;}
inline long hashCode(const Object& x) {return x.hashCode();}
template<class T>
inline long hashCode(const T& x) {return (long)(&x);}

template<class K,class V>
class HashMap : public Object, public Map<K,V> {
private:
	V& null_obj = *((V*)null);
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
        unsigned hc = util::hashCode(k)%mapsize;
        const ArrayList<MapEntry<K,V> >& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.get(i).getKey() == k) {
                return l.get(i).getValue();
            }
        }
        return null_obj;
    }
	V& get(const K& k) {
        unsigned hc = util::hashCode(k)%mapsize;
        ArrayList<MapEntry<K,V>>& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.get(i).getKey() == k) {
                return l.get(i).getRef();
            }
        }
        return null_obj;
	}

    void put(const K& k, const V& v) {
        unsigned hc = util::hashCode(k)%mapsize;
        ArrayList<MapEntry<K,V>>& l=map[hc];
        printf("put hc=%u sz[hc]=%u\n",hc,l.size());
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.get(i).getKey() == k) {
                l.get(i).setVal(v);
                return ;
            }
        }
		MapEntry<K,V> me(k,v);
		printf("add entry\n");
        l.add(me); ++elems;
        printf("put done\n");
    }
    V remove(const K& k) {
        unsigned hc= util::hashCode(k)%mapsize;
        ArrayList<MapEntry<K,V>>& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.get(i).getKey() == k) {
				--elems;
				return l.removeAt(i).getValue();
            }
        }
        return null_obj;
    }
	void clear() {
		elems=0;
	}

	String toString() const {
		if (! elems) return "{}";
		StringBuilder sb;
		sb.append('{');
		for (int i=0; ; ++i) {
            const MapEntry<K,V>& e = entry(i);
			if (&e == null) break;
			const K& key = e.getKey();
			const V& value = e.getValue();
		}
		return sb.append('}').toString();
	}
private:
    void init(unsigned s) {
        map=new ArrayList<MapEntry<K,V> >[s];
        mapsize=s;
    }
    void rehash(unsigned ns) {
    }
    const MapEntry<K,V>& entry(unsigned i) const {
        for (int hc=0; hc<mapsize; ++hc) {
            ArrayList<MapEntry<K,V>>& l=map[hc];
            if (i < l.size()) { return l.get(i); }
            i-=l.size();
        }
        return *((MapEntry<K,V>*)null);
    }
    ArrayList<MapEntry<K,V>> *map;
    unsigned mapsize;
    unsigned elems;
};

} // namespace util

#endif
