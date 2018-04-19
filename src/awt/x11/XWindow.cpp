#include "XWindow.hpp"
#include "XConstants.hpp"
#include "XlibWrapper.hpp"

namespace {
const int MIN_SIZE = 1;

awt::x11::XAtom wm_protocols;
awt::x11::XAtom wm_delete_window;
awt::x11::XAtom wm_take_focus;

awt::x11::XWindow* getParentXWindowObject(awt::Component* target) {
	if (target == null) return null;
	awt::Component* temp = target->getParent();
	if (temp == null) return null;
	awt::ComponentPeer* peer = temp->getPeer();
	if (peer == null) return null;
	while ((peer != null) && !(instanceof<awt::x11::XWindow>(peer))) {
		temp = temp->getParent();
		peer = temp->getPeer();
	}
	if (peer != null && (instanceof<awt::x11::XWindow>(peer))) return (awt::x11::XWindow*) peer;
	return null;
}

}

namespace awt { namespace x11 {

const char* XWindow::TARGET = "target";
const char* XWindow::REPARENTED = "reparented";

XWindow::XWindow(Component* target, long parentWindow, const Rectangle& bounds) : XBaseWindow(
		XCreateWindowParams()
			.put<Long>(TARGET, (long)target)
			.put<Long>(PARENT_WINDOW, parentWindow)
			.put<Rectangle>(BOUNDS, bounds)){}
XWindow::XWindow(long parentWindow) : XBaseWindow(
		XCreateWindowParams()
			.put<Long>(PARENT_WINDOW, parentWindow)
			.put<Boolean>(REPARENTED, Boolean::TRUE)
			.put<Boolean>(EMBEDDED, Boolean::TRUE)){}

void XWindow::initGraphicsConfiguration() {
	LOGD("XWindow::%s: target is %s",__FUNCTION__,target->getClass().getName().cstr());
	graphicsConfig = (X11GraphicsConfig*) &target->getGraphicsConfiguration();
	graphicsConfigData = ((X11GraphicsConfig*)graphicsConfig)->getAData();
}
void XWindow::initWMProtocols() {
	wm_protocols.setAtomListProperty(this, getWMProtocols());

	//do not close connection to XServer when close button(X) is pressed
	//send only ClientMessage event
	long a = wm_delete_window.getAtom();
	XlibWrapper::XSetWMProtocols(XToolkit::getDisplay(), getContentWindow(), &a, 1);
}

void XWindow::preInit(XCreateWindowParams& params) {
	LOGN("XWindow::%s", __FUNCTION__);
	XBaseWindow::preInit(params);
	reparented = Boolean::TRUE.equals(params.get<Boolean>(REPARENTED));

	target = (Component*)params.get<Long>(TARGET).longValue();

	params.putIfNull(EVENT_MASK, XConstants::KeyPressMask | XConstants::KeyReleaseMask
		| XConstants::FocusChangeMask | XConstants::ButtonPressMask | XConstants::ButtonReleaseMask
		| XConstants::EnterWindowMask | XConstants::LeaveWindowMask | XConstants::PointerMotionMask
		| XConstants::ButtonMotionMask | XConstants::ExposureMask | XConstants::StructureNotifyMask);
	if (target != null) {
		params.putIfNull<Rectangle>(BOUNDS, target->getBounds());
	}
	else {
		params.putIfNull<Rectangle>(BOUNDS, Rectangle(0, 0, MIN_SIZE, MIN_SIZE));
	}

	AwtGraphicsConfigData gData = getGraphicsConfigurationData();
	XVisualInfo visInfo = gData.get_awt_visInfo();

	params.putIfNull(BORDER_PIXEL, Long::valueOf(0));
	//params.putIfNull(COLORMAP, gData.get_awt_cmap());
	params.putIfNull(DEPTH, gData.get_awt_depth());
	params.putIfNull<Integer>(VISUAL_CLASS, (int)XConstants::InputOutput);
	params.putIfNull(VISUAL, visInfo.get_visual());
	params.putIfNull<Long>(VALUE_MASK, XConstants::CWBorderPixel | XConstants::CWEventMask | XConstants::CWColormap);
	Long& parentWindow = params.get<Long>(PARENT_WINDOW);
	if (parentWindow == null || parentWindow.longValue() == 0) {
		XToolkit::awtLock();
		Finalize(XToolkit::awtUnlock(););
		int screen = visInfo.get_screen();
		if (screen != -1) params.put(PARENT_WINDOW, XlibWrapper::RootWindow(XToolkit::getDisplay(), screen));
		else params.put(PARENT_WINDOW, XToolkit::getDefaultRootWindow());
	}

	//this->paintArea = new XRepaintArea();
	if (target != null) this->parent = getParentXWindowObject(target->getParent());

	//params.putIfNull(BACKING_STORE, XToolkit::getBackingStoreType());

	LOGN("XWindow::%s", __FUNCTION__);
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	if (!wm_protocols.isSet()) {
		LOGD("getting atoms");
		wm_protocols = XAtom::get("WM_PROTOCOLS");
		wm_delete_window = XAtom::get("WM_DELETE_WINDOW");
		wm_take_focus = XAtom::get("WM_TAKE_FOCUS");
	}
	else {
		LOGD("atoms already set");
	}

	savedState = XUtilConstants::WithdrawnState;
}
void XWindow::postInit(XCreateWindowParams& params) {
	LOGN("XWindow::%s", __FUNCTION__);
	XBaseWindow::postInit(params);

	Color c;
	if (target != null && (c = target->getBackground()) != null) {
		xSetBackground(c);
	}
}

void XWindow::updateSizeHints(int x, int y, int width, int height) {
	long flags = XUtilConstants::PSize | (isLocationByPlatform() ? 0 : (XUtilConstants::PPosition | XUtilConstants::USPosition));
	if (!isResizable()) flags |= XUtilConstants::PMinSize | XUtilConstants::PMaxSize;
	setSizeHints(flags, x, y, width, height);
}
void XWindow::updateSizeHints(int x, int y) {
	long flags = isLocationByPlatform() ? 0 : (XUtilConstants::PPosition | XUtilConstants::USPosition);
	if (!isResizable()) flags |= XUtilConstants::PMinSize | XUtilConstants::PMaxSize;
	setSizeHints(flags, x, y, width, height);
}

void XWindow::xSetBackground(const Color& c) {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	//ColorModel cm = getColorModel();
	//int pixel = PixelConverter.instance.rgbToPixel(c.getRGB(), cm);
	//int pixel = c.getRGB()|0xff000000;
	int pixel = c.getRGB();
	//XlibWrapper::XAllocColor(XToolkit::getDisplay(), XToolkit::getDisplay(), pixel);
	XlibWrapper::XSetWindowBackground(XToolkit::getDisplay(), getContentWindow(), pixel);
	XlibWrapper::XClearWindow(XToolkit::getDisplay(), getContentWindow());
}

}}
