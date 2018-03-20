#ifndef __AWT_PEER_WINDOWPEER_HPP
#define __AWT_PEER_WINDOWPEER_HPP

namespace awt {

class Font;
class Graphics;
class AWTEvent;
class ContainerPeer;
class Frame;
class Component;
class Window;
class Dialog;
class GraphicsConfiguration;

interface MouseInfoPeer : Interface {
public:
	virtual int fillPointWithCoords(Point& point) = 0;
	virtual boolean isWindowUnderMouse(Window* w) = 0;
};

interface ComponentPeer : Interface {
public:
	//virtual boolean isObscured() = 0;
	//virtual boolean canDetermineObscurity() = 0;
	virtual void setVisible(boolean v) = 0;
	virtual void setEnabled(boolean e) = 0;
	virtual void paint(Graphics& g) = 0;
	//virtual void print(Graphics& g) = 0;
	virtual void setBounds(int x, int y, int width, int height, int op) = 0;
	virtual void handleEvent(const AWTEvent& e) = 0;
	virtual Point getLocationOnScreen() = 0;
	virtual Dimension getPreferredSize() = 0;
	virtual Dimension getMinimumSize() = 0;
	//ColorModel getColorModel() = 0;
	virtual Graphics& getGraphics() = 0;
	//FontMetrics getFontMetrics(Font font) = 0;
	virtual void dispose() = 0;
	virtual void setForeground(const Color& c) = 0;
	virtual void setBackground(const Color& c) = 0;
	virtual void setFont(const Font& f) = 0;
	//virtual void updateCursorImmediately() = 0;
	//boolean requestFocus(Component lightweightChild, boolean temporary,
		//                  boolean focusedWindowChangeAllowed, long time,
		//                  CausedFocusEvent.Cause cause) = 0;
	virtual boolean isFocusable() = 0;
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
	virtual void layout() = 0;

	virtual boolean updateGraphicsData(GraphicsConfiguration& gc) = 0;
};
interface CanvasPeer : implements ComponentPeer {
};
interface LightweightPeer : implements ComponentPeer {
};
interface ContainerPeer : implements ComponentPeer {
public:
};
interface PanelPeer : implements ContainerPeer {
};
interface WindowPeer : implements ContainerPeer {
public:
	virtual void toFront() = 0;
	virtual void toBack() = 0;
	//virtual void setModalBlocked(Dialog* blocker, boolean blocked) = 0;
	virtual void updateMinimumSize() = 0;
	//virtual void updateIconImages() = 0;
	//virtual void setOpacity(float opacity) = 0;
	//virtual void setOpaque(boolean isOpaque) = 0;
	virtual void updateWindow() = 0;
	//virtual void repositionSecurityWarning() = 0;
};
interface FramePeer : implements WindowPeer {
public:
	virtual void setTitle(String title) = 0;
};
interface DialogPeer : implements WindowPeer {
public:
	virtual void setTitle(String title) = 0;
};

} // namespace

#endif
