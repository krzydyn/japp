#include "XRootWindow.hpp"

namespace {
awt::x11::XRootWindow *xawtRootWindow = null;
}

namespace awt { namespace x11 {

XRootWindow *XRootWindow::getInstance() {
	if (xawtRootWindow == null) {
		xawtRootWindow = new XRootWindow();
		xawtRootWindow->init();
	}
	return xawtRootWindow;
}

long XRootWindow::getXRootWindow() {
	return getXAWTRootWindow()->getWindow();
}

void XRootWindow::postInit(XCreateWindowParams& params) {
	XBaseWindow::postInit(params);
	//setWMClass(getWMClass());
}

}}
