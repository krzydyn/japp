#ifndef __UTIL_MAP_HPP
#define __UTIL_MAP_HPP

#include <util/ArrayList.hpp>
#include <functional>

namespace util {

template<class K,class V>
class MapEntry {
private:
	K key;
	V value;
public:
	MapEntry(MapEntry&& o) { key = std::move(o.key); value = std::move(o.value); }
	MapEntry(const MapEntry &me) : key(me.key), value(me.value) {}
	MapEntry& operator=(MapEntry&& o) { key = std::move(o.key); value = std::move(o.value); return *this;}
	MapEntry& operator=(const MapEntry& o) { key = o.key; value = o.value; return *this;}

	MapEntry() {}
	MapEntry(const K &k, const V &v) : key(k), value(v) {}
	const K& getKey() const {return key;}
	const V& getValue() const {return value;}
	void setVal(const V& v) {value=v;}
	V& getRef() {return value;}
	String toString() const { return String::valueOf(key) + "=" + String::valueOf(value); }
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
	virtual const V& put(const K& key, const V& value) = 0;
	virtual V remove(const K& key) = 0;
	virtual void clear() = 0;
};

inline jint hash_code(const Object& v) {return v.hashCode();}
template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
inline jint hash_code(const T& v) {
	if (sizeof(v) == 1) return *(uint8_t*)(&v);
	if (sizeof(v) == 2) return *(uint16_t*)(&v);
	if (sizeof(v) == 4) return *(uint32_t*)(&v);

	const long *p = (const long *)&v;
	jint h=0;
	for (unsigned i=0; i < sizeof(v)/sizeof(long); ++i)
		h = 31 * h + p[i];
	return (int)h;

	//when using "const char *p ...", causes a crash, mayby std::hash has the same problem??

	//return std::hash<T>{}(v); // causes a crash (why?)

	//std::hash<T> hasher;
	//return hasher(v);  // causes a crash
}

inline boolean is_equal(const Object& a, const Object& b) { return a.equals(b); }
//SFINAE to choose above function for Object type
template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
inline boolean is_equal(const T& a, const T& b) {return a==b;}

template<class K,class V>
class HashMap : extends Object, implements Map<K,V> {
private:
	V& null_obj = *((V*)null);
	ArrayList<MapEntry<K,V>> *map;
	unsigned mapsize;
	unsigned elems;

	void init(unsigned s) {
		map=new ArrayList<MapEntry<K,V>>[s];
		mapsize=s;
	}
	void rehash(unsigned ns) {
	}
	const MapEntry<K,V>& entry(unsigned i) const {
		for (unsigned hc=0; hc<mapsize; ++hc) {
			ArrayList<MapEntry<K,V>>& l=map[hc];
			if (i < l.size()) { return l.get(i); }
			i-=l.size();
		}
		return *((MapEntry<K,V>*)null);
	}

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
		unsigned hc = util::hash_code(k)%mapsize;
		const ArrayList<MapEntry<K,V> >& l=map[hc];
		for (unsigned i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				return l.get(i).getValue();
			}
		}
		return null_obj;
	}
	V& get(const K& k) {
		unsigned hc = util::hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		for (unsigned i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				return l.get(i).getRef();
			}
		}
		return null_obj;
	}

	const V& put(const K& k, const V& v) {
		unsigned hc = hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		std::cerr << "map.size=" << l.size() << std::endl;
		for (unsigned i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				l.get(i).setVal(v);
				return v;
			}
		}
		MapEntry<K,V> me(k,v);
		l.add(me); ++elems;
		return v;
	}
	V remove(const K& k) {
		unsigned hc = hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		for (unsigned i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
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
		if (!elems) return "{}";
		StringBuilder sb;
		sb.append('{');
		for (unsigned i=0; i < elems; ++i) {
			if (i > 0) sb.append(",");
			const MapEntry<K,V>& e = entry(i);
			if (&e == null) sb.append("<null>");
			else sb.append(e.toString());
		}
		return sb.append('}').toString();
	}
};

template<class K,class V>
using Hashtable = HashMap<K,V>;

} // namespace util

#endif
