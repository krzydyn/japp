#include <awt/Window.hpp>

namespace awt {
namespace {
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
}

LightweightPeer* Toolkit::lightweightMarker = null;

Toolkit& Toolkit::getDefaultToolkit() {
	return (Toolkit&)null_obj;
}

LightweightPeer* Toolkit::createComponent(Component* target) {
	if (lightweightMarker == null) lightweightMarker = new NullComponentPeer();
	return lightweightMarker;
}

Object Component::LOCK;

void Component::invalidateParent() {
	if (parent != null) parent->invalidateIfValid();
}

boolean Component::isShowing() const {
	if (visible && peer != null) {
		return (parent == null) || parent->isShowing();
	}
	return false;
}

Toolkit& Component::getToolkit() {
	Container* parent = this->parent;
	if (parent != null) return parent->getToolkit();
	return Toolkit::getDefaultToolkit();
}

Graphics& Component::getGraphics() {
	if (instanceof<LightweightPeer>(peer)) {
		if (parent == null) return (Graphics&)null_obj;
		Graphics& g = parent->getGraphics();
		if (g == null) return (Graphics&)null_obj;
		g.translate(x,y);
		g.setClip(0, 0, width, height);
		g.setFont(getFont());
		return g;
	}
	else {
		ComponentPeer* peer = this->peer;
		return (peer != null) ? peer->getGraphics() : (Graphics&)null_obj;
	}
}
Font& Component::getFont() {
	Font* font = this->font;
	if (font != null) return *font;
	return (parent != null) ? parent->getFont() : (Font&)null_obj;
}

void Component::addNotify() {
	synchronized (getTreeLock()) {
		ComponentPeer *peer = this->peer;
		if (peer == null || instanceof<LightweightPeer>(peer)) {
			this->peer = peer = getToolkit().createComponent(this);
		}
		else {
			Container* parent = getContainer();
			if (parent != null && parent->isLightweight()) {
				//relocateComponent();
			}
		}
		invalidate();
	}
}

} //namespace
