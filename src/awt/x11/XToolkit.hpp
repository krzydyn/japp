#ifndef __AWT_X11_XTOOLKIT_HPP
#define __AWT_X11_XTOOLKIT_HPP

#include <awt/Window.hpp>
#include <lang/Number.hpp>
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

class XDataWrapper : extends Object {
protected:
	Shared<nio::ByteBuffer> pData;
	XDataWrapper(int size) {
		pData = nio::ByteBuffer::allocate(size);
		pData->order(nio::ByteOrder::LITTLE_ENDIAN);
	}
	XDataWrapper(Shared<nio::ByteBuffer> buf) {
		pData = buf;
		pData->order(nio::ByteOrder::LITTLE_ENDIAN);
	}
	boolean getBool(int offs) const { return pData->get(offs) != 0; }
	void putBool(int offs, boolean b) { pData->put(offs, b?1:0); }
	static int getLongSize() {return 8;}
public:
	long getPData() { return (long)&(pData->array()[0]); }
	void dispose() { pData.reset(); }
};

class XAnyEvent : extends XDataWrapper {
public:
	static int getSize() { return 40; }

	XAnyEvent(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	long get_window() const { return pData->getLong(32); }
};

// in Xlib.h this union
class XEvent : extends XDataWrapper {
public:
	static int getSize() { return 192; }

	XEvent() : XDataWrapper(getSize()) { }
	XEvent(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	int get_type() const { return pData->getInt(0); }
	XAnyEvent get_xany() { return XAnyEvent(pData); }
};

interface DisplayChangedListener : Interface {
};
class X11GraphicsConfig;
class X11GraphicsDevice : extends awt::GraphicsDevice {
private:
	int screen;
	X11GraphicsConfig *defaultConfig = null;
public:
	X11GraphicsDevice(int screennum) : screen(screennum) {}
	awt::GraphicsConfiguration& getDefaultConfiguration();
	int getScreen() { return screen; }
	//long getDisplay();
	void addDisplayChangedListener(DisplayChangedListener* client);
};

class AwtGraphicsConfigData;
class X11GraphicsConfig : extends awt::GraphicsConfiguration {
private:
	AwtGraphicsConfigData *aData;
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

	AwtGraphicsConfigData *getAData() { return aData; }
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
	static String getCorrectXIDString(const String& val);
	static String getEnv(const String& key);

	XToolkit();
	awt::ButtonPeer* createButton(awt::Button* target);
	awt::FramePeer* createLightweightFrame(awt::LightweightFrame* target);
	awt::FramePeer* createFrame(awt::Frame* target);
	awt::LightweightPeer* createComponent(awt::Component* target);
	awt::WindowPeer* createWindow(awt::Window* target);
	awt::DialogPeer* createDialog(awt::Dialog* target);
	awt::EventQueue& getSystemEventQueueImpl();

	awt::MouseInfoPeer& getMouseInfoPeer();

	void run();
	void run(boolean loop);
};

}}

#endif
