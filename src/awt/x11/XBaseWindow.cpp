#include "XBaseWindow.hpp"
#include "XConstants.hpp"
#include "XlibWrapper.hpp"

namespace {
const int MIN_SIZE = 1;
const int DEF_LOCATION = 1;

boolean inManualGrab = false;
awt::x11::Component* componentMouseEnteredRef = null;
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

void XBaseWindow::init(const XCreateWindowParams& params) {
}
void XBaseWindow::create(const XCreateWindowParams& params) {
}
void XBaseWindow::xSetVisible(boolean visible) {
	LOGD(__FUNCTION__);
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	this->visible = visible;
	if (visible) XlibWrapper::XMapWindow(XToolkit::getDisplay(), getWindow());
	else XlibWrapper::XUnmapWindow(XToolkit::getDisplay(), getWindow());
	XlibWrapper::XFlush(XToolkit::getDisplay());
}
void XBaseWindow::xSetBounds(int x, int y, int width, int height) {
	LOGD(__FUNCTION__);
	if (getWindow() == 0) {
		throw IllegalStateException("Attempt to resize uncreated window");
	}
	width = Math.max(MIN_SIZE, width);
	height = Math.max(MIN_SIZE, height);
	
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	XlibWrapper::XMoveResizeWindow(XToolkit::getDisplay(), getWindow(), x,y,width,height);
}

long XBaseWindow::getScreenOfWindow(long window) {
	XToolkit::awtLock();
	Finalize(XToolkit::awtUnlock(););
	return XlibWrapper::getScreenOfWindow(XToolkit::getDisplay(), window);
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
		// we need to call XFlush() here to force ungrab
		// see 6384219 for details
		XlibWrapper::XFlush(XToolkit::getDisplay());
	}
}
void XBaseWindow::dispatchToWindow(const XEvent& ev) {
}
void XBaseWindow::dispatchEvent(const XEvent& ev) {
}

}}
