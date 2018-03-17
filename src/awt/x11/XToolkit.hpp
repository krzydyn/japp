#ifndef __AWT_X11_XTOOLKIT_HPP
#define __AWT_X11_XTOOLKIT_HPP

#include <awt/Window.hpp>
#include <lang/Thread.hpp>

namespace awt { namespace x11 {

class XEvent {
private:
	void *pData;
public:
	XEvent() { }
	XEvent(void *addr) { pData = addr; }
	void *getPData() { return pData; }
};

class XToolkit : extends Toolkit, implements Runnable {
private:
	void init();
	long getNextTaskTime();
public:
	static long getDisplay();
	static long getDefaultRootWindow();

	XToolkit();
	FramePeer* createFrame(Frame* target);
	LightweightPeer* createComponent(Component* target);
	WindowPeer* createWindow(Window* target);
	DialogPeer* createDialog(Dialog* target);

	void run();
	void run(boolean loop);
};

}}

#endif
