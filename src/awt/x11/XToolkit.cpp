#include <lang/Number.hpp>
#include <lang/System.hpp>
#include <util/concurrent/Lock.hpp>

#include "XBaseWindow.hpp"
#include "XConstants.hpp"
#include "XToolkit.hpp"
#include "XlibWrapper.hpp"

// https://www.x.org/docs/X11/xlib.pdf

namespace {

class XMouseInfoPeer : implements awt::MouseInfoPeer {
public:
	XMouseInfoPeer() {};
	int fillPointWithCoords(awt::Point& point) {
		long display = awt::x11::XToolkit::getDisplay();
		awt::GraphicsEnvironment& ge = awt::GraphicsEnvironment::getLocalGraphicsEnvironment();
		Array<awt::GraphicsDevice*> gds = ge.getScreenDevices();

		awt::x11::XToolkit::awtLock();
		Finalize(awt::x11::XToolkit::awtUnlock(););
		for (int i = 0; i < gds.length; i++) {
			long screenRoot = awt::x11::XlibWrapper::RootWindow(display, i);
			long l_root_return, l_child_return;
			long l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return;
			long l_mask_return;
			boolean pointerFound = awt::x11::XlibWrapper::XQueryPointer(display, screenRoot, l_root_return,
					l_child_return, l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return, l_mask_return);
			if (pointerFound) {
				return i;
			}
		}
		LOGE("No pointer found in the system.");
		return -1;
	}
	boolean isWindowUnderMouse(awt::Window* w) {
		long display = awt::x11::XToolkit::getDisplay();
		long contentWindow = ((awt::x11::XWindow*)w->getPeer())->getContentWindow();

		awt::x11::XToolkit::awtLock();
		Finalize(awt::x11::XToolkit::awtUnlock(););
		long screenRoot = awt::x11::XlibWrapper::RootWindow(display, contentWindow);
		long l_root_return, l_child_return;
		long l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return;
		long l_mask_return;
		boolean windowOnTheSameScreen = awt::x11::XlibWrapper::XQueryPointer(display, screenRoot, l_root_return,
					l_child_return, l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return, l_mask_return);
		return windowOnTheSameScreen;
	}
};

const boolean PRIMARY_LOOP = false;
const boolean SECONDARY_LOOP = true;

util::concurrent::ReentrantLock AWT_LOCK;
XMouseInfoPeer xPeer;
Array<awt::GraphicsDevice*> screens;

Thread toolkitThread;
long display;
int arrowCursor;
long eventNumber;
long awt_defaultFg;
HashMap<Long,awt::x11::XBaseWindow*> winMap;

int getNumScreens() {
	return awt::x11::XlibWrapper::XScreenCount(display);
}

awt::GraphicsDevice* makeScreenDevice(int screennum) {
	return new awt::x11::X11GraphicsDevice(screennum);
}

// initialize the X11 display connection
void initDisplay(boolean glxRequested) {
	awt::Toolkit::getDefaultToolkit();
}
int getDefaultScreenNum() {
	return 0;
}

void awt_toolkit_init() {
}
void callTimeoutTasks() {
}
void waitForEvents(long nextTaskTime) {
}

void processException(const Throwable& thr) {
	thr.printStackTrace();
}

awt::Rectangle pGetBounds(int screenNum) {
	//X11GraphicsDevice& dev = screens[screenNum];
	return awt::Rectangle(0,0,1000,1000);
}

}//anonymous namespace

namespace awt { namespace x11 {
X11GraphicsEnvironment::X11GraphicsEnvironment() {
	initDisplay(false);
}
Array<awt::GraphicsDevice*>& X11GraphicsEnvironment::getScreenDevices() {
	LOGD("X11GraphicsEnvironment::%s",__FUNCTION__);
	if (screens.length == 0) {
		int num = getNumScreens();
		screens = Array<awt::GraphicsDevice*>(num);
		for (int i = 0; i < num; i++) {
			screens[i] = makeScreenDevice(i);
		}
	}
	return screens;
}
GraphicsDevice& X11GraphicsEnvironment::getDefaultScreenDevice() {
	LOGD("X11GraphicsEnvironment::%s",__FUNCTION__);
	Array<awt::GraphicsDevice*>& screens = getScreenDevices();
	if (screens.length == 0) throw AWTError("no screen devices");
	int index = getDefaultScreenNum();
	return *screens[0 < index && index < screens.length ? index : 0];
}

const awt::GraphicsConfiguration& X11GraphicsDevice::getDefaultConfiguration() {
	if (defaultConfig == null) {
		defaultConfig = new X11GraphicsConfig(this, 1, 0, 0, false);
	}
	return *defaultConfig;
}

void X11GraphicsConfig::init(int visualNum, int screen) {
}
X11GraphicsConfig::X11GraphicsConfig(X11GraphicsDevice* device, int visualnum, int depth, int colormap, boolean doubleBuffer) {
	this->screen = device;
	this->visual = visualnum;
	this->depth = depth;
	this->colormap = colormap;
	this->doubleBuffer = doubleBuffer;
	init (visualnum, screen->getScreen());
}
GraphicsDevice& X11GraphicsConfig::getDevice() {
	return *screen;
}
Rectangle X11GraphicsConfig::getBounds() const {
	return pGetBounds(screen->getScreen());
}
ColorModel X11GraphicsConfig::getColorModel() const {
	return awt::ColorModel();
}
ColorModel X11GraphicsConfig::getColorModel(int transparency) const {
	return awt::ColorModel();
}


class XComponentPeer : extends XWindow, implements awt::ComponentPeer {
private:
	const GraphicsConfiguration* graphicsConfig = null;
protected:
	XComponentPeer(XCreateWindowParams& params) : XWindow(params) { }
public:
	void setVisible(boolean b) {
		LOGD(__FUNCTION__);
		xSetVisible(b);
	}
	void setEnabled(boolean e) {}
	void paint(Graphics& g) {}
	//virtual void print(Graphics& g) {}
	void setBounds(int x, int y, int width, int height, int op) {
		LOGD("XComponentPeer::%s(%d,%d,%d,%d)", __FUNCTION__, x, y, width, height);
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		xSetBounds(x,y,width,height);
		//validateSurface();
		//layout();
	}
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
	//void updateCursorImmediately() {}
	//boolean requestFocus(Component lightweightChild, boolean temporary,
		//                  boolean focusedWindowChangeAllowed, long time,
		//                  CausedFocusEvent.Cause cause) = 0;
	boolean isFocusable() {return false;}
	//Image createImage(ImageProducer producer) = 0;
	//Image createImage(int width, int height) = 0;
	//boolean prepareImage(Image img, int w, int h, ImageObserver o) = 0;
	//int checkImage(Image img, int w, int h, ImageObserver o) = 0;
	const awt::GraphicsConfiguration& getGraphicsConfiguration() {
		if (graphicsConfig == null) throw NullPointerException("graphicsConfig");
		return *graphicsConfig;
	}
	//virtual boolean handlesWheelScrolling() = 0;
	//void createBuffers(int numBuffers, BufferCapabilities caps) = 0;
	//Image getBackBuffer() = 0;
	//void flip(int x1, int y1, int x2, int y2, BufferCapabilities.FlipContents flipAction) = 0;
	//void destroyBuffers() = 0;
	//virtual void reparent(ContainerPeer& newContainer) = 0;
	//virtual boolean isReparentSupported() = 0;
	void layout() {}
	boolean updateGraphicsData(const awt::GraphicsConfiguration& gc) {
		if (graphicsConfig == &gc) return false;
		graphicsConfig = &gc;
		return true;
	}
};
class XCanvasPeer : extends XComponentPeer, implements awt::CanvasPeer {
protected:
	XCanvasPeer(XCreateWindowParams& params) : XComponentPeer(params) { }
};
class XPanelPeer : extends XCanvasPeer, implements awt::PanelPeer {
protected:
	XPanelPeer(XCreateWindowParams& params) : XCanvasPeer(params) { }
};
class XWindowPeer : extends XPanelPeer, implements awt::WindowPeer {
protected:
	XWindowPeer(XCreateWindowParams& params) : XPanelPeer(params) {
	}
	void preInit(XCreateWindowParams& params) {
		LOGD("XWindowPeer::%s", __FUNCTION__);
		XPanelPeer::preInit(params);
	}
	void postInit(XCreateWindowParams& params) {
		LOGD("XWindowPeer::%s", __FUNCTION__);
		XPanelPeer::postInit(params);
	}
public:
	XWindowPeer(awt::Window* target) : XWindowPeer(
			XCreateWindowParams()
			.put<Long>(TARGET,Long::valueOf((long)target))
			//.put<Long>(PARENT_WINDOW, Long::valueOf(0))
		) {}

	void setVisible(boolean b) {
		LOGD("XWindowPeer::%s(%s)", __FUNCTION__, String::valueOf(b).cstr());
		if (!isVisible() && b) {
			//isBeforeFirstMapNotify = true;
		}
		//updateFocusability();
		//promoteDefaultPosition();
		XPanelPeer::setVisible(b);
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

awt::MouseInfoPeer& XToolkit::getMouseInfoPeer() {
	return xPeer;
}

void XToolkit::addToWinMap(long window, XBaseWindow* xwin) {
	synchronized(winMap) {
		winMap.put(Long::valueOf(window), xwin);
	}
}
void XToolkit::removeFromWinMap(long window, XBaseWindow* xwin) {
	synchronized(winMap) {
		winMap.remove(Long::valueOf(window));
	}
}
XBaseWindow* XToolkit::windowToXWindow(long window) {
	synchronized(winMap) {
		return (XBaseWindow*) winMap.get(Long::valueOf(window));
	}
	return null;
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
	Thread::sleep(1000);
	return -1L;
}

long XToolkit::getDisplay() {
	return display;
}
long XToolkit::getDefaultRootWindow() {
	LOGD("XToolkit::%s", __FUNCTION__);
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
	LOGD("XToolkit::%s", __FUNCTION__);
	awt::FramePeer* peer = new XFramePeer(target);
	targetCreatedPeer(target, peer);
	return peer;
}
awt::LightweightPeer* XToolkit::createComponent(awt::Component* target) {
	LOGD("XToolkit::%s", __FUNCTION__);
	return null;
}
awt::WindowPeer* XToolkit::createWindow(awt::Window* target) {
	LOGD("XToolkit::%s", __FUNCTION__);
	awt::WindowPeer* peer = new XWindowPeer(target);
	targetCreatedPeer(target, peer);
	return peer;
}
awt::DialogPeer* XToolkit::createDialog(awt::Dialog* target) {
	LOGD("XToolkit::%s", __FUNCTION__);
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
