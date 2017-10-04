#ifndef __MAP_HPP
#define __MAP_HPP

template<class K,class V>
class MapEntry {
public:
    MapEntry(const K &key, const V &val) : key(key), val(val) {}
    const K& getKey() const {return key;}
    const V& getVal() const {return val;}
    void setVal(const V& v) {val=v;}
private:
    K key;
    V val;
};

unsigned hashCode(unsigned x) {return x;}
template<class K,class V>
class HashMap {
public:
    HashMap() {init(10);}
    HashMap(unsigned s) {init(s);}
    ~HashMap() { if (map) delete []map; }
    unsigned size() {return elems;}
    const MapEntry<K,V>& entry(unsigned i) {
        for (int hc=0; hc<mapsize; ++hc) {
            List<MapEntry<K,V> >& l=map[hc];
            if (i < l.size()) { return l.ref(i); break;}
            i-=l.size();
        }
        //throw exc.
    }
    void put(const K& k, const V& v) {
        unsigned hc=hashCode(k)%mapsize;
        List<MapEntry<K,V> >& l=map[hc];
        //printf("put %u %u hc=%u sz[hc]=%u\n",k,v,hc,l.size());
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                l.ref(i).setVal(v);
                return ;
            }
        }
        l.add(MapEntry<K,V>(k,v)); ++elems;
    }
    bool get(const K& k, V& v) const {
        unsigned hc=hashCode(k)%mapsize;
        const List<MapEntry<K,V> >& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                v=l.ref(i).getVal();
                return true;
            }
        }
        return false;
    }
    bool remove(const K& k, const V& v) {
        unsigned hc=hashCode(k)%mapsize;
        const List<MapEntry<K,V> >& l=map[hc];
        for (unsigned i=0; i<l.size(); ++i) {
            if (l.ref(i).getKey() == k) {
                v=l.remove(i).getVal(); --elems;
                return true;
            }
        }
        return false;
    }
private:
    void init(unsigned s) {
        map=new List<MapEntry<K,V> >[s];
        mapsize=s;
    }
    void rehash(unsigned ns) {
    }
    List<MapEntry<K,V> > *map;
    unsigned mapsize;
    unsigned elems;
};

#endif
