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
	static const long XA_PRIMARY=1;
	static const long XA_SECONDARY=2;
	static const long XA_ARC=3;
	static const long XA_ATOM=4;
	static const long XA_BITMAP=5;
	static const long XA_CARDINAL=6;
	static const long XA_COLORMAP=7;
	static const long XA_CURSOR=8;
	static const long XA_CUT_BUFFER0=9;
	static const long XA_CUT_BUFFER1=10;
	static const long XA_CUT_BUFFER2=11;
	static const long XA_CUT_BUFFER3=12;
	static const long XA_CUT_BUFFER4=13;
	static const long XA_CUT_BUFFER5=14;
	static const long XA_CUT_BUFFER6=15;
	static const long XA_CUT_BUFFER7=16;
	static const long XA_DRAWABLE=17;
	static const long XA_FONT=18;
	static const long XA_INTEGER=19;
	static const long XA_PIXMAP=20;
	static const long XA_POINT=21;
	static const long XA_RECTANGLE=22;
	static const long XA_RESOURCE_MANAGER=23;
	static const long XA_RGB_COLOR_MAP=24;
	static const long XA_RGB_BEST_MAP=25;
	static const long XA_RGB_BLUE_MAP=26;
	static const long XA_RGB_DEFAULT_MAP=27;
	static const long XA_RGB_GRAY_MAP=28;
	static const long XA_RGB_GREEN_MAP=29;
	static const long XA_RGB_RED_MAP=30;
	static const long XA_STRING=31;
	static const long XA_VISUALID=32;
	static const long XA_WINDOW=33;
	static const long XA_WM_COMMAND=34;
	static const long XA_WM_HINTS=35;
	static const long XA_WM_CLIENT_MACHINE=36;
	static const long XA_WM_ICON_NAME=37;
	static const long XA_WM_ICON_SIZE=38;
	static const long XA_WM_NAME=39;
	static const long XA_WM_NORMAL_HINTS=40;
	static const long XA_WM_SIZE_HINTS=41;
	static const long XA_WM_ZOOM_HINTS=42;
	static const long XA_MIN_SPACE=43;
	static const long XA_NORM_SPACE=44;
	static const long XA_MAX_SPACE=45;
	static const long XA_END_SPACE=46;
	static const long XA_SUPERSCRIPT_X=47;
	static const long XA_SUPERSCRIPT_Y=48;
	static const long XA_SUBSCRIPT_X=49;
	static const long XA_SUBSCRIPT_Y=50;
	static const long XA_UNDERLINE_POSITION=51;
	static const long XA_UNDERLINE_THICKNESS=52 ;
	static const long XA_STRIKEOUT_ASCENT=53;
	static const long XA_STRIKEOUT_DESCENT=54;
	static const long XA_ITALIC_ANGLE=55;
	static const long XA_X_HEIGHT=56;
	static const long XA_QUAD_WIDTH=57;
	static const long XA_WEIGHT=58;
	static const long XA_POINT_SIZE=59;
	static const long XA_RESOLUTION=60;
	static const long XA_COPYRIGHT=61;
	static const long XA_NOTICE=62;
	static const long XA_FONT_NAME=63;
	static const long XA_FAMILY_NAME=64;
	static const long XA_FULL_NAME=65;
	static const long XA_CAP_HEIGHT=66;
	static const long XA_WM_CLASS=67;
	static const long XA_WM_TRANSIENT_FOR=68;
	static const long XA_LAST_PREDEFINED=68;

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
	long getAtom() const { return atom; }
	void putAtom(long ptr) { atom = XAtom::getAtom(ptr); }

	void setProperty(long window, const String& str);
	String getProperty(long window);

	void setPropertyUTF8(long window, const String& str) { setProperty(window, str); }

	void setCard32Property(long window, long value);
	long getCard32Property(long window);

	void deleteProperty(long window);

	Array<XAtom> getAtomListProperty(long window);
	Array<XAtom> getAtomListProperty(XBaseWindow* window);
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
