#ifndef __AWT_X11_XBASEWINDOW_HPP
#define __AWT_X11_XBASEWINDOW_HPP
 
#include <util/HashMap.hpp>
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

class XCreateWindowParams : extends util::HashMap<String,Object*> {
};

class XBaseWindow : extends Object {
private:
	long window = 0;
	boolean visible = false;
	boolean mapped = false;
	boolean embedded = false;
	long screen = 0;

	virtual void create(const XCreateWindowParams& params) final;
protected:
	// Creates an invisible InputOnly window without an associated Component.
	XBaseWindow() : XBaseWindow(XCreateWindowParams()) {}
	XBaseWindow(long parentWindow, const Rectangle& bounds) {}
	XBaseWindow(const Rectangle& bounds) {}
	XBaseWindow(long parentWindow) {}

	virtual void init(const XCreateWindowParams& params) final;
	virtual void ungrabInputImpl() {}
public:
	static long getScreenOfWindow(long window);
	static void ungrabInput();

	XBaseWindow (const XCreateWindowParams& params) { init(params); }

	virtual long getScreenNumber();
	virtual long getScreen();
	virtual long getWindow() { return window; }
    virtual long getContentWindow() { return window; }

	virtual boolean isVisible() { return visible; }
	virtual void xSetBounds(Rectangle bounds) {
		xSetBounds(bounds.x, bounds.y, bounds.width, bounds.height);
	}
	virtual void xSetVisible(boolean visible);
	virtual void xSetBounds(int x, int y, int width, int height);

	static void dispatchToWindow(const XEvent& ev);
	virtual void dispatchEvent(const XEvent& ev);
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
