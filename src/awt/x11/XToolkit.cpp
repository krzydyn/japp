#include <lang/System.hpp>
#include <util/concurrent/Lock.hpp>

#include "XBaseWindow.hpp"
#include "XConstants.hpp"
#include "XToolkit.hpp"
#include "XlibWrapper.hpp"

namespace {

const boolean PRIMARY_LOOP = false;
const boolean SECONDARY_LOOP = true;

util::concurrent::ReentrantLock AWT_LOCK;

Thread toolkitThread;
long display;
int arrowCursor;
long eventNumber;
long awt_defaultFg;

void awt_toolkit_init() {
}
void callTimeoutTasks() {
}
void waitForEvents(long nextTaskTime) {
}

void processException(const Throwable& thr) {
	thr.printStackTrace();
}

}//anonymous namespace

namespace awt { namespace x11 {
class XComponentPeer : extends XWindow, implements awt::ComponentPeer {
	void setVisible(boolean v) {}
	void setEnabled(boolean e) {}
	void paint(Graphics& g) {}
	//virtual void print(Graphics& g) {}
	void setBounds(int x, int y, int width, int height, int op) {}
	void handleEvent(const AWTEvent& e) {}
	Point getLocationOnScreen() {return Point();}
	Dimension getPreferredSize() {return Dimension();}
	Dimension getMinimumSize() {return Dimension();}
	//ColorModel getColorModel() {}
	Graphics& getGraphics() {
		return (Graphics&)null_obj;
	}
	//FontMetrics getFontMetrics(Font font) {}
	void dispose() {}
	void setForeground(const Color& c) {}
	void setBackground(const Color& c) {}
	void setFont(const Font& f) {}
	void updateCursorImmediately() {}
	//boolean requestFocus(Component lightweightChild, boolean temporary,
		//                  boolean focusedWindowChangeAllowed, long time,
		//                  CausedFocusEvent.Cause cause) = 0;
	boolean isFocusable() {return false;}
	//Image createImage(ImageProducer producer) = 0;
	//Image createImage(int width, int height) = 0;
	//boolean prepareImage(Image img, int w, int h, ImageObserver o) = 0;
	//int checkImage(Image img, int w, int h, ImageObserver o) = 0;
	//GraphicsConfiguration getGraphicsConfiguration() = 0;
	//virtual boolean handlesWheelScrolling() = 0;
	//void createBuffers(int numBuffers, BufferCapabilities caps) = 0;
	//Image getBackBuffer() = 0;
	//void flip(int x1, int y1, int x2, int y2, BufferCapabilities.FlipContents flipAction) = 0;
	//void destroyBuffers() = 0;
	//virtual void reparent(ContainerPeer& newContainer) = 0;
	//virtual boolean isReparentSupported() = 0;
	void layout() {}
};
class XCanvasPeer : extends XComponentPeer, implements awt::CanvasPeer {
};
class XPanelPeer : extends XCanvasPeer, implements awt::PanelPeer {
};
class XWindowPeer : extends XPanelPeer, implements awt::WindowPeer {
public:
	XWindowPeer(awt::Window* target) {
	}
	void toFront() {}
	void toBack() {}
	//void setModalBlocked(Dialog* blocker, boolean blocked) = 0;
	void updateMinimumSize() {}
	//void updateIconImages() = 0;
	//void setOpacity(float opacity) = 0;
	//void setOpaque(boolean isOpaque) = 0;
	void updateWindow() {}
	//void repositionSecurityWarning() = 0;
};

class XDecoratedPeer : extends XWindowPeer {
public:
	XDecoratedPeer(awt::Window* target) : XWindowPeer(target) {}
};

class XFramePeer : implements XWindowPeer, implements awt::FramePeer {
public:
	XFramePeer(awt::Frame *target) : XWindowPeer(target) {}
	//XFramePeer(XCreateWindowParams params) : XWindowPeer(params) {}
	void setTitle(String title) {}
};

void notifyListeners(XEvent& ev) {
}
void dispatchEvent(XEvent& ev) {
	//XAnyEvent xany = ev.get_xany();
	if (ev.get_type() == XConstants::MappingNotify) {
	}
	XBaseWindow::dispatchToWindow(ev);
	notifyListeners(ev);
}
void targetCreatedPeer(awt::Component* target, awt::ComponentPeer* peer) {
}

// http://www.grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/awt/X11/XToolkit.java
XToolkit::XToolkit() {
	LOGD("XToolkit construction");
	if (GraphicsEnvironment::isHeadless()) throw HeadlessException();
	display = XlibWrapper::XOpenDisplay("");
	init();
	toolkitThread = Thread(*this, "AWT-XAWT");
	toolkitThread.start();
}

void XToolkit::awtLock() {
	AWT_LOCK.lock();
}
void XToolkit::awtUnlock() {
	AWT_LOCK.unlock();
}

long XToolkit::getNextTaskTime() {
	awtLock();
	Finalize(awtUnlock(););
	//return (Long)timeoutTasks.firstKey();
	return -1L;
}

long XToolkit::getDisplay() {
	return display;
}
long XToolkit::getDefaultRootWindow() {
	awtLock();
	Finalize(awtUnlock(););
	long res = XlibWrapper::RootWindow(XToolkit::getDisplay(), XlibWrapper::DefaultScreen(XToolkit::getDisplay()));
	if (res == 0) throw IllegalStateException("Root window must not be null");
	return res;
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
	LOGD("Cursor created");

	//setup root window
	XlibWrapper::XSelectInput(XToolkit::getDisplay(), XToolkit::getDefaultRootWindow(), (long)XConstants::StructureNotifyMask);
	LOGD("RootWindow selectinput");
}

awt::FramePeer* XToolkit::createFrame(awt::Frame* target) {
	LOGD("%s", __FUNCTION__);
	awt::FramePeer* peer = new XFramePeer(target);
	targetCreatedPeer(target, peer);
	return peer;
}
awt::LightweightPeer* XToolkit::createComponent(awt::Component* target) {
	LOGD("%s", __FUNCTION__);
	return null;
}
awt::WindowPeer* XToolkit::createWindow(awt::Window* target) {
	LOGD("%s", __FUNCTION__);
	awt::WindowPeer* peer = new XWindowPeer(target);
	targetCreatedPeer(target, peer);
	return peer;
}
awt::DialogPeer* XToolkit::createDialog(awt::Dialog* target) {
	LOGD("%s", __FUNCTION__);
	return null;
}

boolean XToolkit::getSunAwtDisableGrab() {
	return false;
}

void XToolkit::run() {
	awt_toolkit_init();
	run(PRIMARY_LOOP);
}
void XToolkit::run(boolean loop) {
	LOGD("%s", __FUNCTION__);
	XEvent ev;
	while(true) {
		if (Thread::currentThread().isInterrupted()) {
			break;
		}
		try {
			awtLock();
			Finalize(awtUnlock(););
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
				XlibWrapper::XNextEvent(getDisplay(), ev.getPData());
			}
			if (ev.get_type() != XConstants::NoExpose) ++eventNumber;
			dispatchEvent(ev);
		}
		catch (const ThreadDeath& td) {
			XBaseWindow::ungrabInput();
			return ;
		}
		catch (const Throwable& thr) {
			XBaseWindow::ungrabInput();
			processException(thr);
		}
	}
}

}}
