#include <awt/Window.hpp>
#include <awt/peer/WindowPeer.hpp>
#include "x11/XToolkit.hpp"

namespace awt {
class NullComponentPeer : extends Object, implements LightweightPeer {
	boolean isObscured() { return false; }
	boolean canDetermineObscurity() { return false; }
	void setVisible(boolean v) {}
	void setEnabled(boolean e) {}
	void paint(Graphics& g) {}
	void print(Graphics& g) {}
	void setBounds(int x, int y, int width, int height, int op) {}
	void handleEvent(const AWTEvent& e) {}
	Point getLocationOnScreen() { return Point(0,0); }
	Dimension getPreferredSize() { return Dimension(1,1); }
	Dimension getMinimumSize() { return Dimension(1,1); }
	//ColorModel getColorModel() = 0;
	Graphics& getGraphics() { return (Graphics&)null_obj; }
	//FontMetrics getFontMetrics(Font font) = 0;
	void dispose() {}
	void setForeground(const Color& c) {}
	void setBackground(const Color& c) {}
	void setFont(const Font& f) {}
	void updateCursorImmediately() {}
	//boolean requestFocus(Component lightweightChild, boolean temporary,
		//                  boolean focusedWindowChangeAllowed, long time,
		//                  CausedFocusEvent.Cause cause) = 0;
	boolean isFocusable() { return false; }
	//Image createImage(ImageProducer producer) = 0;
	//Image createImage(int width, int height) = 0;
	//boolean prepareImage(Image img, int w, int h, ImageObserver o) = 0;
	//int checkImage(Image img, int w, int h, ImageObserver o) = 0;
	//GraphicsConfiguration getGraphicsConfiguration() = 0;
	boolean handlesWheelScrolling() { return false; }
	//void createBuffers(int numBuffers, BufferCapabilities caps) = 0;
	//Image getBackBuffer() = 0;
	//void flip(int x1, int y1, int x2, int y2, BufferCapabilities.FlipContents flipAction) = 0;
	//void destroyBuffers() = 0;
	void reparent(ContainerPeer& newContainer) {}
	boolean isReparentSupported() { return false; }
	void layout() {}
};
class HeadlessToolkit : extends Toolkit {
	FramePeer* createFrame(Frame* target) {
		throw HeadlessException();
	}
	LightweightPeer* createComponent(Component* target) {
		throw HeadlessException();
	}
	WindowPeer* createWindow(Window* target) {
		throw HeadlessException();
	}
	DialogPeer* createDialog(Dialog* target) {
		throw HeadlessException();
	}
};

LightweightPeer* Toolkit::lightweightMarker = null;
Toolkit* Toolkit::toolkit = null;

Toolkit& Toolkit::getDefaultToolkit() {
	if (toolkit == null) {
		if (GraphicsEnvironment::isHeadless()) toolkit = new HeadlessToolkit();
		else toolkit = new x11::XToolkit();
	}
	return *toolkit;
}

LightweightPeer* Toolkit::createComponent(Component* target) {
	if (lightweightMarker == null) lightweightMarker = new NullComponentPeer();
	return lightweightMarker;
}

}
