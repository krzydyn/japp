#include "XAtom.hpp"
#include "XToolkit.hpp"
#include "XlibWrapper.hpp"

namespace {
static util::HashMap<long, awt::x11::XAtom> atomToAtom;
static util::HashMap<String, awt::x11::XAtom> nameToAtom;

const awt::x11::XAtom& lookup(long atom) {
	return atomToAtom.get(atom);
}
const awt::x11::XAtom& lookup(const String& name) {
	return nameToAtom.get(name);
}
}

namespace awt { namespace x11 {

void XAtom::registerAtom(const XAtom& at) {
	if (at.atom != 0) atomToAtom.put(at.atom, at);
	if (!at.name.isEmpty()) nameToAtom.put(at.name, at);
}
XAtom XAtom::get(long atom) {
	const XAtom& xatom = lookup(atom);
	if (xatom == null) return XAtom(atom);
	return xatom;
}
XAtom get(const String& name) {
	const XAtom& xatom = lookup(name);
	if (xatom == null) return XAtom(name, true);
	return xatom;
}

XAtom::XAtom(const String& name, boolean autoIntern) : name(name) {
	this->display = XToolkit::getDisplay();
	if (autoIntern) {
		XToolkit::awtLock();
		this->atom = XlibWrapper::InternAtom(display,name,0);
		XToolkit::awtUnlock();
	}
	registerAtom(*this);
}
XAtom::XAtom(long atom) : atom(atom) {
	this->display = XToolkit::getDisplay();
	registerAtom(*this);
}

String XAtom::getName() const {
	if (name.isEmpty()) {
		XToolkit::awtLock();
		const_cast<XAtom*>(this)->name = XlibWrapper::XGetAtomName(display, atom);
		XToolkit::awtUnlock();
	}
	return name;
}
void XAtom::setProperty(long window, const String& str) {
	if (atom == 0) throw IllegalStateException("Atom should be initialized");
	XToolkit::awtLock();
	XlibWrapper::SetProperty(display,window,atom,str);
	XToolkit::awtUnlock();
}
String XAtom::getProperty(long window) {
	if (atom == 0) throw IllegalStateException("Atom should be initialized");
	XToolkit::awtLock();
	String prop = XlibWrapper::GetProperty(display,window,atom);
	XToolkit::awtUnlock();
	return prop;
}
void XAtom::deleteProperty(long window) {
	if (atom == 0) throw IllegalStateException("Atom should be initialized");
	XToolkit::awtLock();
	XlibWrapper::XDeleteProperty(display,window,atom);
	XToolkit::awtUnlock();
}


}}
