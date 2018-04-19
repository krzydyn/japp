#include <lang/System.hpp>
#include <util/concurrent/Lock.hpp>

#include "XAtom.hpp"
#include "XConstants.hpp"
#include "XToolkit.hpp"
#include "XWindow.hpp"
#include "XlibWrapper.hpp"

#include <unistd.h>  //pipe
#include <fcntl.h>
#include <poll.h>

// X11 know-how
// https://www.x.org/docs/X11/xlib.pdf
// http://neuron-ai.tuke.sk/hudecm/Tutorials/C/special/xlib-programming/xlib-programming-2.html
// https://www.x.org/releases/X11R7.6/doc/libX11/specs/libX11/libX11.html
// https://github.com/JetBrains/jdk8u_jdk/blob/master/src/solaris/native/sun/xawt/XToolkit.c

namespace {

class XWM final {
public:
	static const int UNDETERMINED_WM = 1;
	static const int NO_WM = 2;
	static const int OTHER_WM = 3;
	static const int OPENLOOK_WM = 4;
	static const int MOTIF_WM = 5;
	static const int CDE_WM = 6;
	static const int ENLIGHTEN_WM = 1;
	static const int KDE2_WM = 8;
	static const int SAWFISH_WM = 9;
	static const int ICE_WM = 10;
	static const int METACITY_WM = 11;
	static const int COMPIZ_WM = 12;
	static const int LG3D_WM = 13;
	static const int CWM_WM = 14;
	static const int MUTTER_WM = 15;
	//static const int _WM = 16;

	static int getWMID();
};

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

const char *eventTypeName[35] = {
	"Reserved",
	"Reserved",
	"KeyPress",
	"KeyRelease",
	"ButtonPress",
	"ButtonRelease",
	"MotionNotify",
	"EnterNotify",
	"LeaveNotify",
	"FocusIn",
	"FocusOut",
	"KeymapNotify",
	"Expose",
	"GraphicsExpose",
	"NoExpose",
	"VisibilityNotify",
	"CreateNotify",
	"DestroyNotify",
	"UnmapNotify",
	"MapNotify",
	"MapRequest",
	"ReparentNotify",
	"ConfigureNotify",
	"ConfigureRequest",
	"GravityNotify",
	"ResizeRequest",
	"CirculateNotify",
	"CirculateRequest",
	"PropertyNotify",
	"SelectionClear",
	"SelectionRequest",
	"SelectionNotify",
	"ColormapNotify",
	"ClientMessage",
	"MappingNotify"
};

String eventType(const awt::x11::XEvent& ev) {
	int t = ev.get_type();
	if (t < 0 || t > 34) return String::format("Event type %d", t);
	return eventTypeName[t];
}

#define AWT_READPIPE            (awt_pipe_fds[0])
#define AWT_WRITEPIPE           (awt_pipe_fds[1])

const boolean PRIMARY_LOOP = false;
const boolean SECONDARY_LOOP = true;
const int DEF_AWT_FLUSH_TIMEOUT = 100;
const int DEF_AWT_MAX_POLL_TIMEOUT = 1000;

util::concurrent::ReentrantLock AWT_LOCK;
int awt_wmgr = XWM::UNDETERMINED_WM;
long awt_display;
long awt_defaultFg;
int AWT_FLUSH_TIMEOUT    = DEF_AWT_FLUSH_TIMEOUT; /* milliseconds */
int AWT_MAX_POLL_TIMEOUT = DEF_AWT_MAX_POLL_TIMEOUT; /* milliseconds */
int  awt_pipe_fds[2];                   /* fds for wkaeup pipe */
boolean awt_pipe_inited = false;
long awt_last_flush_time = 0L;
long awt_next_flush_time = 0L;
struct ::pollfd pollFds[2];

XMouseInfoPeer xPeer;
Array<awt::GraphicsDevice*> screens;
Thread toolkitThread;
int arrowCursor;
long eventNumber;

int get_poll_timeout(jlong nextTaskTime) {
	jlong curTime = System.currentTimeMillis();
	return nextTaskTime == -1 ? AWT_MAX_POLL_TIMEOUT : (int)(nextTaskTime - curTime);
}
void awt_pipe_init() {
	if (awt_pipe_inited) return ;
	if (::pipe(awt_pipe_fds) == 0) {
		int32_t flags;
		flags = fcntl(AWT_READPIPE, F_GETFL, 0);
		::fcntl(AWT_READPIPE, F_SETFL, flags | O_NDELAY | O_NONBLOCK);
		flags = fcntl(AWT_WRITEPIPE, F_GETFL, 0);
		::fcntl(AWT_WRITEPIPE, F_SETFL, flags | O_NDELAY | O_NONBLOCK);
		awt_pipe_inited = true;
	}
	else {
		AWT_READPIPE = -1;
		AWT_WRITEPIPE = -1;
	}
}
void awt_toolkit_init() {
	awt_pipe_init();
}


HashMap<Long,void*> timeoutTasks() {
	static HashMap<Long,void*> *map = null;
	if (map == null) map = new HashMap<Long,void*>();
	return *map;
}
HashMap<Long,awt::x11::XBaseWindow*>& winMap() {
	static HashMap<Long,awt::x11::XBaseWindow*> *map = null;
	if (map == null) map = new HashMap<Long,awt::x11::XBaseWindow*>();
	return *map;
}

int getNumScreens() {
	return awt::x11::XlibWrapper::XScreenCount(awt_display);
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

void callTimeoutTasks() {
}

void performPoll(long nextTaskTime) {
	static boolean pollFdsInited = false;

	int timeout = get_poll_timeout(nextTaskTime);
	if (!pollFdsInited) {
		pollFds[0].fd = awt::x11::XlibWrapper::XConnectionNumber(awt_display);
		LOGD("Connection number = %d", pollFds[0].fd);
		pollFds[0].events = POLLIN;
		pollFds[0].revents = 0;
		pollFds[1].fd = AWT_READPIPE;
		pollFds[1].events = POLLIN;
		pollFds[1].revents = 0;
		pollFdsInited = true;
	}
	else {
		pollFds[0].revents = 0;
		pollFds[1].revents = 0;
	}
	if (timeout == 0) Thread::yield();

	int result = poll(pollFds, 2, timeout);
	if (result == 0) {
		LOGD("poll timeout");
		return ;
	}
	if (pollFds[1].revents) {
		LOGD("Woke up");
		char read_buf[10];
		ssize_t count;
		do {
			count = read(AWT_READPIPE, read_buf, sizeof(read_buf));
		} while (count == sizeof(read_buf));
	}
	if (pollFds[0].revents) {
		LOGD("Events in X pipe");
	}
}
void waitForEvents(long nextTaskTime) {
	performPoll(nextTaskTime);
	if ((awt_next_flush_time > 0) && (System.currentTimeMillis() >= awt_next_flush_time)) {
		awt::x11::XlibWrapper::XFlush(awt_display);
		awt_last_flush_time = awt_next_flush_time;
		//awt_next_flush_time = 0L;
		awt_next_flush_time = System.currentTimeMillis() + AWT_FLUSH_TIMEOUT;
	}
}

void processException(const Throwable& thr) {
	thr.printStackTrace();
	_exit(0);
}

awt::Rectangle pGetBounds(int screenNum) {
	//X11GraphicsDevice& dev = screens[screenNum];
	return awt::Rectangle(0,0,1000,1000);
}

int XWM::getWMID() {
	if (awt_wmgr != XWM::UNDETERMINED_WM) return awt_wmgr;

	awt::x11::XToolkit::awtLock();
	Finalize(awt::x11::XToolkit::awtUnlock(););

	awt_wmgr = XWM::OTHER_WM;

	return awt_wmgr;
}

#define HOST_NAME_MAX 1024
String getLocalHostname() {
	char hostname[HOST_NAME_MAX + 1];
	if (gethostname(hostname, HOST_NAME_MAX + 1) == 0) {
		hostname[HOST_NAME_MAX] = '\0';
		return hostname;
    }
	return "";
}
int getJvmPID() {
	return getpid();
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
		LOGD("Number of screens is %d", num);
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

awt::GraphicsConfiguration& X11GraphicsDevice::getDefaultConfiguration() {
	if (defaultConfig == null) {
		defaultConfig = new X11GraphicsConfig(this, 1, 0, 0, false);
	}
	return *defaultConfig;
}
void X11GraphicsDevice::addDisplayChangedListener(DisplayChangedListener* client) {
	//TODO displayChanger.add(client);
}

void X11GraphicsConfig::init(int visualNum, int screen) {
	//aData = nio::ByteBuffer::allocate(AwtGraphicsConfigData::getSize());
	aData = new AwtGraphicsConfigData;
	//TODO read AwtGraphicsConfigData from X11
}
X11GraphicsConfig::X11GraphicsConfig(X11GraphicsDevice* device, int visualnum, int depth, int colormap, boolean doubleBuffer) {
	this->screen = device;
	this->visual = visualnum;
	this->depth = depth;
	this->colormap = colormap;
	this->doubleBuffer = doubleBuffer;
	init(visualnum, screen->getScreen());
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
	GraphicsConfiguration* graphicsConfig = null;
protected:
	int boundsOperation;

	XComponentPeer(XCreateWindowParams& params) : XWindow(params) { }
	virtual boolean isInitialReshape() { return true; }
	void preInit(XCreateWindowParams& params) override {
		XWindow::preInit(params);
		boundsOperation = DEFAULT_OPERATION;
	}
	void postInit(XCreateWindowParams& params) override {
		XWindow::postInit(params);
		if (isInitialReshape()) {
			LOGN("XComponentPeer::%s initial reshape",__FUNCTION__);
			Rectangle r = target->getBounds();
			reshape(r.x, r.y, r.width, r.height);
		}
		else {
			LOGN("XComponentPeer::%s NO initial reshape",__FUNCTION__);
		}
		setEnabled(target->isEnabled());
		if (target->isVisible()) setVisible(true);
	}
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
	void reshape(int x, int y, int width, int height) {
		setBounds(x, y, width, height, SET_BOUNDS);
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
	awt::GraphicsConfiguration& getGraphicsConfiguration() {
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
	boolean updateGraphicsData(awt::GraphicsConfiguration& gc) {
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
class XWindowPeer : extends XPanelPeer, implements awt::WindowPeer, implements DisplayChangedListener {
private:
	//static Set<XWindowPeer> windows = new HashSet<XWindowPeer>();
	Window::Type windowType = Window::Type::NORMAL;

	void updateShape() {
	}
	void updateOpacity() {
	}

	// src/solaris/classes/sun/awt/X11/XWindowPeer.java
	void promoteDefaultPosition() {
		boolean locationByPlatform = ((Window*)target)->isLocationByPlatform();
		if (locationByPlatform) {
			XToolkit::awtLock();
			Finalize(XToolkit::awtUnlock(););
			Rectangle bounds = getBounds();
			long f = 0;
			setSizeHints(f & ~(XUtilConstants::USPosition | XUtilConstants::PPosition),
				bounds.x, bounds.y, bounds.width, bounds.height);
		}
	}

protected:
	XWindowPeer(XCreateWindowParams& params) : XPanelPeer(params) {
		LOGN("XWindowPeer::XWindowPeer created");
	}
	boolean isSimpleWindow() {
		return !(instanceof<Frame>(target) || instanceof<Dialog>(target));
	}
	boolean isOverrideRedirect() {
		return XWM::getWMID() == XWM::OPENLOOK_WM || Window::Type::POPUP == getWindowType();
	}
	void preInit(XCreateWindowParams& params) override {
		LOGN("XWindowPeer::%s", __FUNCTION__);
		target = (Component*)params.get<Long>(TARGET).longValue();
		windowType = ((Window*)target)->getType();
		params.put<Boolean>(REPARENTED, Boolean::valueOf(isOverrideRedirect() || isSimpleWindow()));
		XPanelPeer::preInit(params);

		params.putIfNull(BIT_GRAVITY, Integer::valueOf(XConstants::NorthWestGravity));

		long eventMask = 0;
		if (params.containsKey(EVENT_MASK)) eventMask = params.get<Long>(EVENT_MASK);
		eventMask |= XConstants::VisibilityChangeMask;
		params.put<Long>(EVENT_MASK, eventMask);

		//TODO XA_NET_WM_STATE = XAtom::get("_NET_WM_STATE");

		params.put(OVERRIDE_REDIRECT, Boolean::valueOf(isOverrideRedirect()));

		//TODO windows.add(this);

		//TODO Font f = target->getFont();
		
		//TODO Color c = targeti->getBackground();

		//TODO awt::GraphicsConfiguration& gc = getGraphicsConfiguration();
		//((X11GraphicsDevice&)gc.getDevice()).addDisplayChangedListener(this);
	}
	void postInit(XCreateWindowParams& params) override {
		LOGN("XWindowPeer::%s", __FUNCTION__);
		XPanelPeer::postInit(params);
	
		initWMProtocols();

		XAtom::get("WM_CLIENT_MACHINE").setProperty(getWindow(), getLocalHostname());
		XAtom::get("_NET_WM_PID").setCard32Property(getWindow(), getJvmPID());

		updateIconImages();
		updateShape();
		updateOpacity();
	}
public:
	XWindowPeer(awt::Window* target) : XWindowPeer(
			XCreateWindowParams()
			.put<Long>(TARGET,Long::valueOf((long)target))
			//.put<Long>(PARENT_WINDOW, Long::valueOf(0))
		) {}

	Window::Type getWindowType() { return windowType; }
	void setVisible(boolean b) override {
		LOGN("XWindowPeer::%s(%s)", __FUNCTION__, String::valueOf(b).cstr());
		if (!isVisible() && b) {
			//isBeforeFirstMapNotify = true;
		}
		//updateFocusability();
		promoteDefaultPosition();
		XPanelPeer::setVisible(b);
	}
	void toFront() {}
	void toBack() {}
	//void setModalBlocked(Dialog* blocker, boolean blocked) = 0;
	void updateMinimumSize() {}
	void updateIconImages()  {}
	void setOpacity(float opacity) {}
	//void setOpaque(boolean isOpaque) = 0;
	void updateWindow() {}
	//void repositionSecurityWarning() = 0;
	void setBounds(int x, int y, int width, int height, int op) override {
		XToolkit::awtLock();
		Finalize(XToolkit::awtUnlock(););
		XPanelPeer::setBounds(x, y, width, height, op);
		setSizeHints(XUtilConstants::PPosition | XUtilConstants::PSize, x, y, width, height);
	}
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
	LOGD("dispatchEvent type=%d '%s'", ev.get_type(), eventType(ev).cstr());
	//XAnyEvent xany = ev.get_xany();
	if (ev.get_type() == XConstants::MappingNotify) {
	}
	XBaseWindow::dispatchToWindow(ev);
	notifyListeners(ev);

	//if ev not cleared
	switch (ev.get_type()) {
	case XConstants::ClientMessage:
		//if (ev.xclient.message_type == wm_protocols &&
		//	ev.xclient.data.l[0] == wm_delete_window)  {
			Thread::currentThread().interrupt();
		//}
		break;
	}
}
void targetCreatedPeer(awt::Component* target, awt::ComponentPeer* peer) {
	if (target != null && peer != null && !awt::GraphicsEnvironment::isHeadless()) {
		//AWTAutoShutdown::getInstance().registerPeer(target, peer);
	}
}

// http://www.grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/awt/X11/XToolkit.java
XToolkit::XToolkit() {
	LOGD("XToolkit construction");
	if (GraphicsEnvironment::isHeadless()) throw HeadlessException();
	awt_display = XlibWrapper::XOpenDisplay("");
	init();
	toolkitThread = Thread(*this, "AWT-XAWT");
	toolkitThread.start();
}

awt::MouseInfoPeer& XToolkit::getMouseInfoPeer() {
	return xPeer;
}

void XToolkit::addToWinMap(long window, XBaseWindow* xwin) {
	LOGN("addToWinMap win=%lX", window);
	synchronized(winMap()) {
		winMap().put(Long::valueOf(window), xwin);
	}
}
void XToolkit::removeFromWinMap(long window, XBaseWindow* xwin) {
	synchronized(winMap()) {
		winMap().remove(Long::valueOf(window));
	}
}
XBaseWindow* XToolkit::windowToXWindow(long window) {
	synchronized(winMap()) {
		if (winMap().containsKey(Long::valueOf(window)))
			return (XBaseWindow*) winMap().get(Long::valueOf(window));
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
	if (timeoutTasks().isEmpty()) {
		return -1L;
	}
	//return (Long)timeoutTasks.firstKey();
	LOGD("XToolkit::getNextTaskTime ret t0+100");
	return System.currentTimeMillis() + 100;
}

long XToolkit::getDisplay() {
	return awt_display;
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
	if (awt_display == 0) throw HeadlessException();

	LOGD("XToolkit::init");

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

	//TODO call somewhere else XBaseWindow::getXAWTRootWindow();
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
	XWindowPeer* peer = new XWindowPeer(target);
	LOGD("XToolkit::%s created peer %s", __FUNCTION__, peer->getClass().getName().cstr());
	peer->init();
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
	//TODO make it better
	Thread::sleep(2); // time to initialize static vars in main thread

	awt_toolkit_init();
	run(PRIMARY_LOOP);
}
void XToolkit::run(boolean loop) {
	LOGD("%s(%s)", __FUNCTION__, String::valueOf(loop).cstr());
	XEvent ev;

	while(true) {
		if (Thread::currentThread().isInterrupted()) {
			break;
		}
		awtLock();
		Finalize(awtUnlock(););
		try {
			if (loop == SECONDARY_LOOP) {
				if (!XlibWrapper::XNextSecondaryLoopEvent(getDisplay(), ev.getPData())) {
					break;
				}
			}
			else {
				callTimeoutTasks();
				while ( (XlibWrapper::XEventsQueued(getDisplay(), XConstants::QueuedAlready) == 0) &&
						(XlibWrapper::XEventsQueued(getDisplay(), XConstants::QueuedAfterReading) == 0) &&
						(XlibWrapper::XEventsQueued(getDisplay(), XConstants::QueuedAfterFlush) == 0)) {
					callTimeoutTasks();
					//LOGD("Toolkit::loop call waitForEvents");
					waitForEvents(getNextTaskTime());
				}
				XlibWrapper::XNextEvent(getDisplay(), ev.getPData());
			}
			if (ev.get_type() != XConstants::NoExpose) ++eventNumber;
			long w = 0;
			if (windowToXWindow(ev.get_xany().get_window()) != null) {
				/*Component* owner = XKeyboardFocusManagerPeer::getInstance().getCurrentFocusOwner();
				if (owner != null) {
					XWindow ownerWindow = (XWindow) AWTAccessor::getComponentAccessor().getPeer(owner);
					if (ownerWindow != null) w = ownerWindow.getContentWindow();
				}*/
			}
			if (XlibWrapper::XFilterEvent(ev.getPData(), w)) continue;
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
	if (loop == PRIMARY_LOOP) {
		LOGD("AWT loop finished");
		XlibWrapper::XCloseDisplay(awt_display);
	}
}

}}
