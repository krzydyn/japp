#include <lang/System.hpp>

#include "XToolkit.hpp"
#include "XlibWrapper.hpp"

namespace {
const boolean PRIMARY_LOOP = false;
const boolean SECONDARY_LOOP = true;

Thread toolkitThread;
long display;
int arrowCursor;
long awt_defaultFg;

void awt_toolkit_init() {
}
}

namespace awt { namespace x11 {

// http://www.grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/awt/X11/XToolkit.java
long XToolkit::getDisplay() {
	return display;
}
long XToolkit::getDefaultRootWindow() {
	long res = XlibWrapper::RootWindow(XToolkit::getDisplay(), XlibWrapper::DefaultScreen(XToolkit::getDisplay()));
	if (res == 0) throw IllegalStateException("Root window must not be null");
	return res;
}

XToolkit::XToolkit() {
	LOGD("XToolkit construction");
	if (GraphicsEnvironment::isHeadless()) throw HeadlessException();
	display = XlibWrapper::XOpenDisplay("");
	init();
	toolkitThread = Thread(*this, "AWT-XAWT");
	toolkitThread.start();
}

void XToolkit::init() {
	if (display == 0) throw HeadlessException();

	XlibWrapper::XSupportsLocale();
	if (XlibWrapper::XSetLocaleModifiers("") == null) {
		LOGE("X locale modifiers are not supported, using default");
	}

	int screen = XlibWrapper::DefaultScreen(XToolkit::getDisplay());
	awt_defaultFg = XlibWrapper::BlackPixel(XToolkit::getDisplay(), screen);

	//setup cursor
	arrowCursor = XlibWrapper::XCreateFontCursor(XToolkit::getDisplay(), (int)XCursorFontConstants::XC_arrow);

	//setup root window
	XlibWrapper::XSelectInput(XToolkit::getDisplay(), XToolkit::getDefaultRootWindow(), (long)XConstants::StructureNotifyMask);
}

FramePeer* XToolkit::createFrame(Frame* target) {
	return null;
}
LightweightPeer* XToolkit::createComponent(Component* target) {
	return null;
}
WindowPeer* XToolkit::createWindow(Window* target) {
	return null;
}
DialogPeer* XToolkit::createDialog(Dialog* target) {
	return null;
}

void XToolkit::run() {
	awt_toolkit_init();
	run(PRIMARY_LOOP);
}
void XToolkit::run(boolean loop) {
	XEvent ev;
	while(true) {
		if (Thread::currentThread().isInterrupted()) {
			break;
		}
		if (loop == SECONDARY_LOOP) {
			if (!XlibWrapper::XNextSecondaryLoopEvent(getDisplay(), ev.getPData())) {
				break;
			}
		}
		else {
			callTimeoutTasks();
			while ((XlibWrapper::XEventsQueued(getDisplay(), XConstants::QueuedAfterReading) == 0) &&
					(XlibWrapper::XEventsQueued(getDisplay(), XConstants::QueuedAfterFlush) == 0)) {
				callTimeoutTasks();
				waitForEvents(getNextTaskTime());
			}
			XlibWrapper::XNextEvent(getDisplay(),ev.pData);
		}
	}
}

}}
