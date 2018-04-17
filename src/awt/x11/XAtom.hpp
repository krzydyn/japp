#ifndef __AWT_X11_XATOM_HPP
#define __AWT_X11_XATOM_HPP

#include <lang/String.hpp>
#include <util/ArrayList.hpp>

namespace awt { namespace x11 {

class XAtomList;
class XBaseWindow;
class XAtom final : extends Object {
private:
	long atom = 0;
	String name;
	long display; //FIXME maybe remove this field

	void registerAtom(const XAtom& at);

	void setAtomListProperty(long window, const Array<XAtom>& atoms);
	void setAtomListProperty(long window, const XAtomList& atoms);
public:
	static const long XA_CARDINAL=6;
	static int getAtomSize() { return 8; }
	static long getAtom(long ptr);
	static XAtom get(long atom);
	static XAtom get(const String& name);

	XAtom(const XAtom& o) : atom(o.atom), name(o.name), display(o.display) {}
 	XAtom() {}
	XAtom(long atom);
	XAtom(const String& name, boolean autoIntern);

	boolean equals(const Object& o) const override {
		if (!(instanceof<const XAtom>(&o))) return false;
		const XAtom& ot = (const XAtom&)o;
		return (atom == ot.atom && display == ot.display);
	}
	jint hashCode() const override {
		return (jint)((atom ^ display) & 0xFFFFL);
	}

	boolean isSet() {return atom!=0;}

	String getName() const;

	void setProperty(long window, const String& str);
	String getProperty(long window);

	void setCard32Property(long window, long value);
	long getCard32Property(long window);

	void deleteProperty(long window);

	void setAtomListProperty(XBaseWindow* window, const Array<XAtom>& atoms);
	void setAtomListProperty(XBaseWindow* window, const XAtomList& atoms);
};

class XAtomList : extends Object {
private:
	ArrayList<XAtom> atoms;
	void init(long data, int count);
	void init(const Array<XAtom>& atoms);
public:
	XAtomList(XAtomList&& o) : atoms(std::move(o.atoms)) {}

	XAtomList() {}
	XAtomList(long data, int count) { init(data, count); }
	XAtomList(const Array<XAtom>& atoms) { init(atoms); }
	Array<XAtom> getAtoms();
	long getAtomsData();

	boolean contains(const XAtom& atom) { return atoms.contains(atom); }
	void remove(const XAtom& atom) { atoms.remove(atom); }
	int size() { return atoms.size(); }
	void add(const XAtom& atom) { atoms.add(atom); }
};

}}

#endif
