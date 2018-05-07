#include <lang/Number.hpp>
#include <awt/Window.hpp>

#include "XAtom.hpp"
#include "XBaseWindow.hpp"
#include "XConstants.hpp"
#include "XlibWrapper.hpp"
#include "XRootWindow.hpp"

namespace {
const int MIN_SIZE = 1;
const int DEF_LOCATION = 1;

boolean inManualGrab = false;
awt::Component* componentMouseEnteredRef = null;
awt::x11::XBaseWindow* grabWindowRef = null;

void _setGrabWindow(awt::x11::XBaseWindow* grabWindow, boolean isAutoGrab) {
	awt::x11::XToolkit::awtLock();
	Finalize(awt::x11::XToolkit::awtUnlock(););

	if (inManualGrab && isAutoGrab) return ;
	inManualGrab = grabWindow != null && !isAutoGrab;
	if (grabWindow == null) {
		grabWindowRef = null;
		return ;
	}
	grabWindowRef = grabWindow;
}

}

namespace awt { namespace x11 {

class XSetWindowAttributes : extends XDataWrapper {
public:
	static int getSize() { return 112; }
	XSetWindowAttributes() : XDataWrapper(getSize()) {}
	XSetWindowAttributes(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	long get_background_pixmap() { return pData->getLong(0); }
	void set_background_pixmap(long v) {  pData->putLong(0, v); }
	long get_background_pixel() { return (pData->getLong(8)); }
	void set_background_pixel(long v) {  pData->putLong(8, v); }
	long get_border_pixmap() { return (pData->getLong(16)); }
	void set_border_pixmap(long v) {  pData->putLong(16, v); }
	long get_border_pixel() { return (pData->getLong(24)); }
	void set_border_pixel(long v) {  pData->putLong(24, v); }
	int get_bit_gravity() { return (pData->getInt(32)); }
	void set_bit_gravity(int v) {  pData->putInt(32, v); }
	int get_win_gravity() { return (pData->getInt(36)); }
	void set_win_gravity(int v) {  pData->putInt(36, v); }
	int get_backing_store() { return (pData->getInt(40)); }
	void set_backing_store(int v) {  pData->putInt(40, v); }
	long get_backing_planes() { return (pData->getLong(48)); }
	void set_backing_planes(long v) {  pData->putLong(48, v); }
	long get_backing_pixel() { return (pData->getLong(56)); }
	void set_backing_pixel(long v) {  pData->putLong(56, v); }
	boolean get_save_under() { return (getBool(64)); }
	void set_save_under(boolean v) {  putBool(64, v); }
	long get_event_mask() { return (pData->getLong(72)); }
	void set_event_mask(long v) {  pData->putLong(72, v); }
	long get_do_not_propagate_mask() { return (pData->getLong(80)); }
	void set_do_not_propagate_mask(long v) {  pData->putLong(80, v); }
	boolean get_override_redirect() { return (getBool(88)); }
	void set_override_redirect(boolean v) { putBool(88, v); }
	long get_colormap() { return (pData->getLong(96)); }
	void set_colormap(long v) {  pData->putLong(96, v); }
	long get_cursor() { return (pData->getLong(104)); }
	void set_cursor(long v) {  pData->putLong(104, v); }
};

void XAwtState::setComponentMouseEntered(Component* component) {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	if (component == null) {
		componentMouseEnteredRef = null;
		return ;
	}
	componentMouseEnteredRef = component;
}
Component* XAwtState::getComponentMouseEntered() {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	if (componentMouseEnteredRef == null) return null;
	return componentMouseEnteredRef;
}
boolean XAwtState::isManualGrab() {
	return inManualGrab;
}
void XAwtState::setGrabWindow(XBaseWindow* grabWindow) {
	_setGrabWindow(grabWindow, false);
}
void XAwtState::setAutoGrabWindow(XBaseWindow* grabWindow) {
	_setGrabWindow(grabWindow, true);
}
XBaseWindow *XAwtState::getGrabWindow() {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	return grabWindowRef;
}

XCreateWindowParams::~XCreateWindowParams() {
	//params
}

const char* XBaseWindow::PARENT_WINDOW = "parent window";
const char* XBaseWindow::BOUNDS = "bounds";
const char* XBaseWindow::OVERRIDE_REDIRECT = "overrideRedirect";
const char* XBaseWindow::EVENT_MASK = "event mask";
const char* XBaseWindow::VALUE_MASK = "value mask";
const char* XBaseWindow::BORDER_PIXEL = "border pixel";
const char* XBaseWindow::COLORMAP = "color map";
const char* XBaseWindow::DEPTH = "visual depth";
const char* XBaseWindow::VISUAL_CLASS = "visual class";
const char* XBaseWindow::VISUAL = "visual";
const char* XBaseWindow::EMBEDDED = "embedded";
const char* XBaseWindow::DELAYED = "delayed";
const char* XBaseWindow::PARENT = "parent";
const char* XBaseWindow::BACKGROUND_PIXMAP = "pixmap";
const char* XBaseWindow::VISIBLE = "visible";
const char* XBaseWindow::SAVE_UNDER = "save under";
const char* XBaseWindow::BACKING_STORE = "backing store";
const char* XBaseWindow::BIT_GRAVITY = "bit gravity";

String XBaseWindow::getWMName() {
	return XToolkit::getCorrectXIDString(getClass().getName());
}
void XBaseWindow::updateWMName() {
	String name = getWMName();
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	XAtom nameAtom = XAtom::get(XAtom::XA_WM_NAME);
	nameAtom.setProperty(getWindow(), name);
	XAtom netNameAtom = XAtom::get("_NET_WM_NAME");
	netNameAtom.setPropertyUTF8(getWindow(), name);
}
void XBaseWindow::ungrabInput() {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););

	XBaseWindow* grabWindow = XAwtState::getGrabWindow();
	if (grabWindow != null) {
		grabWindow->ungrabInputImpl();
		if (!XToolkit::getSunAwtDisableGrab()) {
			XlibWrapper::XUngrabPointer(XToolkit::getDisplay(), XConstants::CurrentTime);
			XlibWrapper::XUngrabKeyboard(XToolkit::getDisplay(), XConstants::CurrentTime);
		}
		XAwtState::setGrabWindow(null);
		XlibWrapper::XFlush(XToolkit::getDisplay());
	}
}
XRootWindow *XBaseWindow::getXAWTRootWindow() {
	return XRootWindow::getInstance();
}
long XBaseWindow::getScreenOfWindow(long window) {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	return XlibWrapper::getScreenOfWindow(XToolkit::getDisplay(), window);
}

XBaseWindow::XBaseWindow(XCreateWindowParams& params) : delayedParams(params) {
	//init(parms) moved to XToolkit::createWindow as peer->init();
}
XBaseWindow::XBaseWindow(long parentWindow, const Rectangle& bounds) : XBaseWindow(
	XCreateWindowParams()
		.put<Rectangle>(BOUNDS, bounds)
		.put<Long>(PARENT_WINDOW, parentWindow)) {}
XBaseWindow::XBaseWindow(const Rectangle& bounds) : XBaseWindow(
	XCreateWindowParams()
		.put<Rectangle>(BOUNDS, bounds)) {}
XBaseWindow::XBaseWindow(long parentWindow) : XBaseWindow(
	XCreateWindowParams()
		.put<Long>(PARENT_WINDOW, parentWindow)) {}

void XBaseWindow::preInit(XCreateWindowParams& params) {
	LOGN("XBaseWindow::%s", __FUNCTION__);
	initialising = InitialiseState::NOT_INITIALISED;
	embedded = Boolean::TRUE.equals(params.get<Boolean>(EMBEDDED));
	visible = Boolean::TRUE.equals(params.get<Boolean>(VISIBLE));

	Object* parent = null;
	Long& l = params.get<Long>(PARENT);
	if (l != null) parent = (Object*)l.longValue();
	if (instanceof<XBaseWindow>(parent)) {
		parentWindow = (XBaseWindow*)parent;
		LOGD("  parentWondow is XBaseWindow");
	}
	else {
		Long& parentWindowID = params.get<Long>(PARENT_WINDOW);
		if (parentWindowID != null) {
			LOGD("  parentWindow is id=%d", parentWindowID.longValue());
			parentWindow = XToolkit::windowToXWindow(parentWindowID);
		}
		else {
			LOGD("  parentWindow is null");
		}
	}

	Long& eventMask = params.get<Long>(EVENT_MASK);
	if (eventMask != null) {
		long mask = eventMask.longValue();
		mask |= XConstants::SubstructureNotifyMask;
		params.put<Long>(EVENT_MASK, mask);
	}

	screen = -1;
}
void XBaseWindow::postInit(XCreateWindowParams& params) {
	//updateWMName();
	//initClientLeader();
}
void XBaseWindow::init() {
	XCreateWindowParams params(delayedParams);
	init(params);
}
void XBaseWindow::init(XCreateWindowParams& params) {
	initialising = InitialiseState::INITIALISING;
	if (!Boolean::TRUE.equals(params.get<Boolean>(DELAYED))) {
		LOGD("initializing Window: %s", getClass().getName().cstr());
		preInit(params);
		create(params);
		postInit(params);
	}
	initialising = InitialiseState::INITIALISED;
}
void XBaseWindow::checkParams(XCreateWindowParams& params) {
	params.putIfNull<Long>(PARENT_WINDOW, Long::valueOf(XToolkit::getDefaultRootWindow()));
	params.putIfNull<Rectangle>(BOUNDS, Rectangle(DEF_LOCATION, DEF_LOCATION, MIN_SIZE, MIN_SIZE));
	params.putIfNull<Integer>(DEPTH, Integer::valueOf((int)XConstants::CopyFromParent));
	params.putIfNull<Long>(VISUAL, Long::valueOf(XConstants::CopyFromParent));
	params.putIfNull<Integer>(VISUAL_CLASS, Integer::valueOf((int)XConstants::InputOnly));
	params.putIfNull<Long>(VALUE_MASK, Long::valueOf(XConstants::CWEventMask));

	Rectangle& bounds = params.get<Rectangle>(BOUNDS);
	bounds.width = Math::max(MIN_SIZE, bounds.width);
	bounds.height = Math::max(MIN_SIZE, bounds.height);

	Long& eventMaskObj = params.get<Long>(EVENT_MASK);
	long eventMask = eventMaskObj != null ? eventMaskObj.longValue() : 0;
	eventMask |= XConstants::PropertyChangeMask | XConstants::OwnerGrabButtonMask;
	params.put<Long>(EVENT_MASK, Long::valueOf(eventMask));
}
void XBaseWindow::create(XCreateWindowParams& params) {
	LOGD("XBaseWindow::%s", __FUNCTION__);
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););

	checkParams(params);

	XSetWindowAttributes xattr;

	long value_mask = params.get<Long>(VALUE_MASK).longValue();

	Long& eventMask = params.get<Long>(EVENT_MASK);
	xattr.set_event_mask(eventMask.longValue());
	value_mask |= XConstants::CWEventMask;

	// clear bits which will be rebuilt
	value_mask &= ~(XConstants::CWBorderPixel | XConstants::CWColormap | XConstants::CWBackPixmap);

	Long& border_pixel = params.get<Long>(BORDER_PIXEL);
	if (border_pixel != null) {
		LOGD("xattr: add BORDER_PX");
		xattr.set_border_pixel(border_pixel.longValue());
		value_mask |= XConstants::CWBorderPixel;
	}

	Long& colormap = params.get<Long>(COLORMAP);
	if (colormap != null) {
		LOGD("xattr: add COLORMAP");
		xattr.set_colormap(colormap.longValue());
		value_mask |= XConstants::CWColormap;
	}

	Long& background_pixmap = params.get<Long>(BACKGROUND_PIXMAP);
	if (background_pixmap != null) {
		LOGD("xattr: add BG_PIXMAP");
		xattr.set_background_pixmap(background_pixmap.longValue());
		value_mask |= XConstants::CWBackPixmap;
	}

	Long& parentWindow = params.get<Long>(PARENT_WINDOW);
	Rectangle& bounds = params.get<Rectangle>(BOUNDS);
	Integer& depth = params.get<Integer>(DEPTH);
	Integer& visual_class = params.get<Integer>(VISUAL_CLASS);
	Long& visual = params.get<Long>(VISUAL);

	Boolean& overrideRedirect = params.get<Boolean>(OVERRIDE_REDIRECT);
	if (overrideRedirect != null) {
		xattr.set_override_redirect(overrideRedirect.booleanValue());
		value_mask |= XConstants::CWOverrideRedirect;
	}

	Boolean& saveUnder = params.get<Boolean>(SAVE_UNDER);
	if (saveUnder != null) {
		xattr.set_save_under(saveUnder.booleanValue());
		value_mask |= XConstants::CWSaveUnder;
	}

	Integer& backingStore = params.get<Integer>(BACKING_STORE);
	if (backingStore != null) {
		xattr.set_backing_store(backingStore.intValue());
		value_mask |= XConstants::CWBackingStore;
	}

	window = XlibWrapper::XCreateWindow(XToolkit::getDisplay(), parentWindow.longValue(),
				bounds.x, bounds.y, // location
				bounds.width, bounds.height, // size
				0, // border width
				depth.intValue(), // depth
				visual_class.intValue(), // class
				visual.longValue(), // visual
				value_mask,         // value mask
				xattr.getPData());  // attributes
	if (window == 0) throw IllegalStateException("Couldn't create window because of wrong parameters. Run with NOISY_AWT to see details");

	XToolkit::addToWinMap(window, this);
}

void XBaseWindow::xSetVisible(boolean visible) {
	LOGD("XBaseWindow::%s",__FUNCTION__);
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	this->visible = visible;
	if (visible) {
		XlibWrapper::XMapWindow(XToolkit::getDisplay(), getWindow());
		//XlibWrapper::XMoveWindow(XToolkit::getDisplay(), getWindow(), x, y);
	}
	else XlibWrapper::XUnmapWindow(XToolkit::getDisplay(), getWindow());
	XlibWrapper::XFlush(XToolkit::getDisplay());
}
void XBaseWindow::xSetBounds(int x, int y, int width, int height) {
	if (getWindow() == 0) {
		throw IllegalStateException("Attempt to resize uncreated window");
	}
	width = Math::max(MIN_SIZE, width);
	height = Math::max(MIN_SIZE, height);
	LOGD("%s(%d,%d,%d,%d)",__FUNCTION__,x,y,width,height);
/*
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;
*/
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	XlibWrapper::XMoveResizeWindow(XToolkit::getDisplay(), getWindow(),x,y,width,height);
}
void XBaseWindow::setSizeHints(long flags, int x, int y, int width, int height) {
	LOGD("%s(f=%X, %d,%d,%d,%d)",__FUNCTION__, flags,x,y,width,height);
	XSizeHints hints;

	if ((flags & XUtilConstants::PPosition) != 0) {
		hints.set_x(x);
		hints.set_y(y);
	}

	if ((flags & XUtilConstants::PSize) != 0) {
		hints.set_width(width);
		hints.set_height(height);
	}
	else if ((hints.get_flags() & XUtilConstants::PSize) != 0) {
		flags |= XUtilConstants::PSize;
	}

	if ((flags & XUtilConstants::PMinSize) != 0) {
		hints.set_min_width(width);
		hints.set_min_height(height);
	} else if ((hints.get_flags() & XUtilConstants::PMinSize) != 0) {
		flags |= XUtilConstants::PMinSize;
	}

	flags |= XUtilConstants::PWinGravity;
	hints.set_flags(flags);
	hints.set_win_gravity((int)XConstants::NorthWestGravity);

	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	XlibWrapper::XSetWMNormalHints(XToolkit::getDisplay(), getWindow(), hints.getPData());
}

long XBaseWindow::getScreenNumber() {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	return XlibWrapper::XScreenNumberOfScreen(getScreen());
}
long XBaseWindow::getScreen() {
	if (screen == -1) {
		screen = getScreenOfWindow(window);
	}
	return screen;
}
void XBaseWindow::dispatchToWindow(const XEvent& ev) {
}
void XBaseWindow::dispatchEvent(const XEvent& ev) {
}

}}
