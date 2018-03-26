#ifndef __AWT_X11_XTOOLKIT_HPP
#define __AWT_X11_XTOOLKIT_HPP

#include <awt/Window.hpp>
#include <lang/Thread.hpp>
#include <nio/ByteBuffer.hpp>

namespace awt { namespace x11 {

// http://www.grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/awt/X11GraphicsEnvironment.java
// iX11GraphicsEnvironment -> SunGraphicsEnvironment -> GraphicsEnvironment
class X11GraphicsEnvironment : extends GraphicsEnvironment {
public:
	X11GraphicsEnvironment();
	Array<GraphicsDevice*>& getScreenDevices();
	GraphicsDevice& getDefaultScreenDevice();
	boolean isDisplayLocal() {return true;}
};

class XEvent : extends Object {
private:
	Shared<nio::ByteBuffer> pData;
public:
	static int getSize() { return 192; }

	XEvent() {
		pData = nio::ByteBuffer::allocate(getSize());
	}
	XEvent(Shared<nio::ByteBuffer> buf) {
		pData = buf;
	}
	void *getPData() { return &(pData->array()[0]); }
	void dispose() { pData.reset(); }
	int get_type() { return pData->getInt(0); }
};

class X11GraphicsConfig;
class X11GraphicsDevice : extends awt::GraphicsDevice {
private:
	int screen;
	X11GraphicsConfig *defaultConfig = null;
public:
	X11GraphicsDevice(int screennum) : screen(screennum) {}
	const awt::GraphicsConfiguration& getDefaultConfiguration();
	int getScreen() { return screen; }
	//long getDisplay();
};

class X11GraphicsConfig : extends awt::GraphicsConfiguration {
private:
	void init(int visualNum, int screen);
protected:
	X11GraphicsDevice* screen;
	int visual;
	int depth;
	int colormap;
	boolean doubleBuffer;
public:
	X11GraphicsConfig(X11GraphicsDevice* device, int visualnum, int depth, int colormap, boolean doubleBuffer);
	//deprecated, replaced by device->getDefaultConfig(), TODO by device->getConfig(v,d,cm,type)
	//static X11GraphicsConfig& getConfig(X11GraphicsDevice* device, int visualnum, int depth, int colormap, int type);

	GraphicsDevice& getDevice();
	ColorModel getColorModel() const;
	ColorModel getColorModel(int transparency) const;
	//virtual AffineTransform getDefaultTransform();
	//virtual AffineTransform getNormalizingTransform();
	Rectangle getBounds() const;
	boolean isTranslucencyCapable() const {return false;}
};

class XBaseWindow;
class XToolkit : extends Toolkit, implements Runnable {
private:
	void init();
	long getNextTaskTime();
public:
	static void awtLock();
	static void awtUnlock();
	static long getDisplay();
	static long getDefaultRootWindow();
	static boolean getSunAwtDisableGrab();
	static void addToWinMap(long window, XBaseWindow* xwin);
	static void removeFromWinMap(long window, XBaseWindow* xwin);
	static XBaseWindow* windowToXWindow(long window);
	//static void addEventDispatcher(long window, XEventDispatcher dispatcher);

	XToolkit();
	awt::FramePeer* createFrame(awt::Frame* target);
	awt::LightweightPeer* createComponent(awt::Component* target);
	awt::WindowPeer* createWindow(awt::Window* target);
	awt::DialogPeer* createDialog(awt::Dialog* target);

	awt::MouseInfoPeer& getMouseInfoPeer();

	void run();
	void run(boolean loop);
};

}}

#endif
