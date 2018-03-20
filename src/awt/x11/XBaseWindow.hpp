#ifndef __AWT_X11_XBASEWINDOW_HPP
#define __AWT_X11_XBASEWINDOW_HPP
 
#include "XToolkit.hpp"

namespace awt { namespace x11 {

class XBaseWindow;
class Component;
class XAwtState {
public:
	static void setComponentMouseEntered(Component* component);
	static Component* getComponentMouseEntered();
	static boolean isManualGrab();
	static void setGrabWindow(XBaseWindow* grabWindow);
	static void setAutoGrabWindow(XBaseWindow* grabWindow);
	static XBaseWindow *getGrabWindow();
};

class XBaseWindow : extends Object {
protected:
	int window;
	virtual void ungrabInputImpl() {}
public:
	static void ungrabInput();
	static void dispatchToWindow(const XEvent& ev);

	virtual void dispatchEvent(const XEvent& ev);
	virtual int getWindow() { return window; }
    virtual int getContentWindow() { return window; }
};

class XWindow : extends XBaseWindow {
public:
	XWindow() {}
	XWindow(Component* target, long parentWindow, const Rectangle& bounds) {
	}
	XWindow(Object* target) : XWindow() {
	}
	XWindow(long parentWindow) {
	}
};

}}

#endif
