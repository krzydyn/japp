#ifndef __UTIL_MAP_HPP
#define __UTIL_MAP_HPP

#include <util/ArrayList.hpp>
#include <functional>

namespace util {

template<class K,class V>
class MapEntry : extends Object {
private:
	K key;
	V value;
	void move(MapEntry& o) {
		key = std::move(o.key);
		value = std::move(o.value);
	}
public:
	MapEntry(MapEntry&& o) { move(o); }
	MapEntry(const MapEntry &o) : key(o.key), value(o.value) {}
	MapEntry& operator=(MapEntry&& o) { move(o); return *this;}
	MapEntry& operator=(const MapEntry& o) { key = o.key; value = o.value; return *this;}

	MapEntry() {}
	MapEntry(const K &k, const V &v) : key(k), value(v) {}
	const K& getKey() const {return key;}
	const V& getValue() const {return value;}
	void setVal(const V& v) {value=v;}
	V& getRef() {return value;}
	String toString() const {TRACE; return String::valueOf(key) + ":" + String::valueOf(value); }
};

template<class K,class V>
interface Map : Interface {
public:
	virtual ~Map() {}
	virtual int size() const = 0;
	virtual boolean isEmpty() const final {return size() == 0;}
	virtual boolean containsKey(const K& key) const = 0;
	virtual boolean containsValue(const V& value) const = 0;
	virtual const V& get(const K& key) const = 0;
	virtual V& get(const K& key) = 0;
	virtual const V& put(const K& key, const V& value) = 0;
	virtual V remove(const K& key) = 0;
	virtual void clear() = 0;
};

//SFINAE to choose function for Object type
template<class T, class std::enable_if<std::is_base_of<Object,T>::value,Object>::type* = nullptr>
inline unsigned hash_code(const T& v) {return (unsigned)v.hashCode();}
template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
inline unsigned hash_code(const T& v) {
	if (sizeof(v) == 1) return *(uint8_t*)(&v);
	if (sizeof(v) == 2) return *(uint16_t*)(&v);
	if (sizeof(v) == 4) return *(uint32_t*)(&v);

	const uint32_t *p = (const uint32_t *)&v;
	jint h=0;
	for (unsigned i=0; i < sizeof(v)/sizeof(uint32_t); ++i)
		h = 31 * h + p[i];
	return (unsigned)h;

	//when using "const char *p ...", causes a crash, maybe std::hash has the same problem??

	//return std::hash<T>{}(v); // causes a crash (why?)

	//std::hash<T> hasher;
	//return hasher(v);  // causes a crash
}

//SFINAE to choose function for Object type
inline boolean is_equal(const Object& a, const Object& b) { return a.equals(b); }
template<class T, class std::enable_if<!std::is_base_of<Object,T>::value,Object>::type* = nullptr>
inline boolean is_equal(const T& a, const T& b) {return a == b;}

template<class K,class V>
class HashMap : extends Object, implements Map<K,V> {
public:
	static const int DEFAULT_INITIAL_CAPACITY = 1 << 4;
	static const int MAXIMUM_CAPACITY = 1 << 30;
	static constexpr float DEFAULT_LOAD_FACTOR = 0.75f;

private:
	ArrayList<MapEntry<K,V>> *map;
	unsigned mapsize;
	unsigned _size = 0;
	//float loadFactor = DEFAULT_LOAD_FACTOR;

	void init(unsigned s) {TRACE;
		mapsize=s;
		map=new ArrayList<MapEntry<K,V>>[s];
	}
	void rehash(unsigned ns) {TRACE;
		if (ns == mapsize) return ;
		ArrayList<MapEntry<K,V>> *nmap = new ArrayList<MapEntry<K,V>>[ns];
		if (nmap == null) return ; //throw RuntimeException("Out of memory");
		for (int i=0; i < _size; ++i) {
			MapEntry<K,V>* e = const_cast<MapEntry<K,V>*>(entry(i));
			unsigned hc = util::hash_code(e->getKey())%ns;
			nmap[hc].add(std::move(*e));
		}
		delete [] map;
		map = nmap;
		mapsize = ns;
	}
	const MapEntry<K,V>* entry(int i) const {TRACE;
		for (unsigned hc=0; hc < mapsize; ++hc) {
			ArrayList<MapEntry<K,V>>& l = map[hc];
			if (i < l.size()) { return &l.get(i); }
			i -= l.size();
		}
		return null;
	}

public:
	HashMap(const HashMap& o) {
		init(o.mapsize);
		for (unsigned i=0; i < o.mapsize; ++i) {
			map[i].addAll(o.map[i]);
		}
	}

	HashMap() {TRACE;init(DEFAULT_INITIAL_CAPACITY);}
	HashMap(unsigned s) {TRACE;init(s);}
	~HashMap() {TRACE;
		clear();
		if (map) delete []map;
	}

	int size() const override {return (int)_size;}
	boolean containsKey(const K& key) const {TRACE;
		unsigned hc = util::hash_code(key)%mapsize;
		const ArrayList<MapEntry<K,V> >& l=map[hc];
		for (int i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), key)) return true;
		}
		return false;
	}
	boolean containsValue(const V& value) const {TRACE;
		return false;
	}

	const V& get(const K& k) const {TRACE;
		unsigned hc = util::hash_code(k)%mapsize;
		const ArrayList<MapEntry<K,V> >& l=map[hc];
		for (int i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				return l.get(i).getValue();
			}
		}
		//std::cerr << "key(const) not found: " << String::valueOf(k).intern() << ", return null_obj" << std::endl;
		//if (sizeof(V) <= sizeof(void*))  return (const V&)null_val;
		return (const V&)null_obj;
	}
	V& get(const K& k) {TRACE;
		unsigned hc = util::hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		for (int i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				return l.get(i).getRef();
			}
		}
		//std::cerr << "key not found: " << String::valueOf(k).intern() << ", return null_obj" << std::endl;
		throw NullPointerException("key not found: " + String::valueOf(k));
		return (V&)null_obj;
	}

	const V& put(const K& k, const V& v) {TRACE;
		unsigned hc = hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		for (int i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				l.get(i).setVal(v);
				return v;
			}
		}
		MapEntry<K,V> me(k,v);
		l.add(me); ++_size;
		return v;
	}
	V remove(const K& k) {TRACE;
		unsigned hc = hash_code(k)%mapsize;
		ArrayList<MapEntry<K,V>>& l=map[hc];
		for (int i=0; i<l.size(); ++i) {
			if (is_equal(l.get(i).getKey(), k)) {
				--_size;
				return l.removeAt(i).getValue();
			}
		}
		return *((V*)&null_obj);
	}
	void clear() {TRACE;
		if (map)
			for (unsigned i=0; i < mapsize; ++i) map[i].clear();
		_size=0;
	}

	String toString() const {TRACE;
		if (!_size) return "{}";
		StringBuilder sb;
		sb.append("["+String::valueOf(_size)+"] {");
		for (int i=0; i < (int)_size; ++i) {
			if (i > 0) sb.append(",");
			const MapEntry<K,V>* e = entry(i);
			if (e == null) sb.append("<null>");
			else sb.append(e->toString());
		}
		return sb.append('}').toString();
	}
};

template<class K,class V>
using Hashtable = HashMap<K,V>;

} // namespace util

#endif
