#ifndef __AWT_X11_XWINDOW_HPP
#define __AWT_X11_XWINDOW_HPP
 
#include "XAtom.hpp"
#include "XBaseWindow.hpp"

namespace awt {
namespace x11 {

class XWindowAttributesData {
public:
    boolean nativeDecor;
    boolean initialFocus;
    boolean isResizable;
    int initialState;
    boolean initialResizability;
    int visibilityState; // updated by native X11 event handling code.
    String title;
    //util::ArrayList<IconInfo*> icons;
    boolean iconsInherited;
    int decorations;            // for future expansion to be able to
                                // specify native decorations
    int functions; // MWM_FUNC_*
};

class XWindow : extends XBaseWindow {
private:
	int savedState;
	boolean reparented;
	XWindow *parent;

	awt::GraphicsConfiguration *graphicsConfig = null;
	AwtGraphicsConfigData *graphicsConfigData = null;
protected:
	awt::Component *target;
	XWindowAttributesData winAttr;

	XWindow() { throw RuntimeException("not supp"); }
	XWindow(XCreateWindowParams& params) : XBaseWindow(params) {}
	XWindow(awt::Component* target, long parentWindow, const Rectangle& bounds);
	XWindow(Object* target) { throw RuntimeException("not supp"); }
	XWindow(long parentWindow);

	void initGraphicsConfiguration();
	virtual void initWMProtocols() final;
	virtual XAtomList getWMProtocols() { return XAtomList(); }
	void preInit(XCreateWindowParams& params) override;
	void postInit(XCreateWindowParams& params) override;

	boolean isShowing() { return visible; }
	boolean isResizable() { return true; }
	boolean isLocationByPlatform() { return false; }

	void updateSizeHints() { updateSizeHints(x, y, width, height); }
	void updateSizeHints(int x, int y, int width, int height);
	void updateSizeHints(int x, int y);

public:
	static const char* TARGET;
	static const char* REPARENTED;

	void xSetBackground(const Color& c);
	virtual GraphicsConfiguration& getGraphicsConfiguration() {
		if (graphicsConfig == null) initGraphicsConfiguration();
		if (graphicsConfig == null) throw RuntimeException("Can't get GraphicsConfiguration");
		return *graphicsConfig;
	}
	virtual const AwtGraphicsConfigData& getGraphicsConfigurationData() {
		if (graphicsConfigData == null) initGraphicsConfiguration();
		if (graphicsConfigData == null) throw RuntimeException("Can't get GraphicsConfigurationData");
		return *graphicsConfigData;
	}
};

}}

#endif
