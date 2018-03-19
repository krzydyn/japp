#include "XBaseWindow.hpp"
#include "XConstants.hpp"
#include "XlibWrapper.hpp"

namespace {
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
