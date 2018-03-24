#ifndef __AWT_X11_XBASEWINDOW_HPP
#define __AWT_X11_XBASEWINDOW_HPP
 
#include <util/HashMap.hpp>
#include "XToolkit.hpp"

namespace awt {
class Component;
namespace x11 {

class XBaseWindow;
class XAwtState {
public:
	static void setComponentMouseEntered(Component* component);
	static awt::Component* getComponentMouseEntered();
	static boolean isManualGrab();
	static void setGrabWindow(XBaseWindow* grabWindow);
	static void setAutoGrabWindow(XBaseWindow* grabWindow);
	static XBaseWindow *getGrabWindow();
};

class XCreateWindowParams : extends util::HashMap<String,Object*> {
public:
	XCreateWindowParams(const XCreateWindowParams& o) : util::HashMap<String,Object*>(o) {
	}
	XCreateWindowParams() {}

	template <class T>
	XCreateWindowParams& putIfNull(const String& key, const T& value) {
		if (value != null) {
			Object *o = new T(value);
			if (!containsKey(key)) util::HashMap<String,Object*>::put(key, o);
		}
		return *this;
	}
	template <class T>
	XCreateWindowParams& put(const String& key, const T& value) {
		if (value != null) {
			Object *o = new T(value);
			util::HashMap<String,Object*>::put(key, o);
		}
		return *this;
	}
	template <class T>
	T& get(const String& key) const {
		if (!containsKey(key)) return (T&)null_obj;
		Object *o = util::HashMap<String,Object*>::get(key);
		return (T&)(*o);
	}
};

class XBaseWindow : extends Object {
private:
	XCreateWindowParams delayedParams;

	long window = 0;
	boolean visible = false;
	boolean mapped = false;
	boolean embedded = false;
	XBaseWindow* parentWindow;

	long screen = 0;

	enum class InitialiseState {
		INITIALISING,
		NOT_INITIALISED,
		INITIALISED,
		FAILED_INITIALISATION
	};
	InitialiseState initialising;

protected:
	int x;
	int y;
	int width;
	int height;

	// Creates an invisible InputOnly window without an associated Component.
	//XBaseWindow() : XBaseWindow(XCreateWindowParams()) {}
	XBaseWindow() {} //dummy

	XBaseWindow(long parentWindow, const Rectangle& bounds);
	XBaseWindow(const Rectangle& bounds) {}
	XBaseWindow(long parentWindow) {}

	virtual void init(XCreateWindowParams& params) final;
	virtual void checkParams(XCreateWindowParams& params);
	virtual void preInit(XCreateWindowParams& params);
	virtual void create(XCreateWindowParams& params);
	virtual void postInit(XCreateWindowParams& params);
	virtual void ungrabInputImpl() {}

public:
    static const char* PARENT_WINDOW; // parent window, Long
    static const char* BOUNDS; // bounds of the window, Rectangle
    static const char* OVERRIDE_REDIRECT; // override_redirect setting, Boolean
    static const char* EVENT_MASK; // event mask, Integer
    static const char* VALUE_MASK; // value mask, Long
    static const char* BORDER_PIXEL; // border pixel value, Integer
    static const char* COLORMAP; // color map, Long
    static const char* DEPTH; // depth, Integer
    static const char* VISUAL_CLASS; // visual class, Integer
    static const char* VISUAL; // visual, Long
    static const char* EMBEDDED; // is embedded?, Boolean
    static const char* DELAYED; // is creation delayed?, Boolean
    static const char* PARENT; // parent peer
    static const char* BACKGROUND_PIXMAP; // background pixmap
    static const char* VISIBLE; // whether it is visible by default
    static const char* SAVE_UNDER; // save content under this window
    static const char* BACKING_STORE; // enables double buffering
    static const char* BIT_GRAVITY; // copy old content on geometry change

	static long getScreenOfWindow(long window);
	static void ungrabInput();

	XBaseWindow(XCreateWindowParams& params) { init(params); }

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
private:
	int savedState;
	boolean reparented;
	XWindow *parent;
	awt::Component *target;

protected:
	XWindow() { throw RuntimeException("not supp"); }
	XWindow(XCreateWindowParams& params) { init(params); }
	XWindow(awt::Component* target, long parentWindow, const Rectangle& bounds);
	XWindow(Object* target) { throw RuntimeException("not supp"); }
	XWindow(long parentWindow);

	void preInit(XCreateWindowParams& params) override;
	void postInit(XCreateWindowParams& params) override;
public:
	static const char* TARGET;
	static const char* REPARENTED;

	void xSetBackground(const Color& c);
};

}}

#endif
