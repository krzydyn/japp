#include <awt/Window.hpp>
#include <lang/System.hpp>

namespace awt {

Object Component::LOCK;

void Component::repaintParentIfNeeded(int oldX, int oldY, int oldWidth, int oldHeight) {
	if (parent != null && instanceof<LightweightPeer>(peer) && isShowing()) {
		parent->repaint(oldX, oldY, oldWidth, oldHeight);
		repaint();
	}
}
void Component::notifyNewBounds(boolean resized, boolean moved) {
	//TODO send events to children
}

void Component::invalidateParent() {
	if (parent != null) parent->invalidateIfValid();
}

void Component::repaint(long tm, int x, int y, int width, int height) {
	if (instanceof<LightweightPeer>(peer)) {
		if (parent != null) {
			if (x < 0) { width += x; x = 0; }
			if (y < 0) { height += y; y = 0; }
			int pwidth = (width > this->width) ? this->width : width;
			int pheight = (height > this->height) ? this->height : height;
			if (pwidth <= 0 || pheight <= 0) return ;
			int px = this->x + x;
			int py = this->y + y;
			parent->repaint(tm, px, py, pwidth, pheight);
		}
	}
	else {
		if (isVisible() && (peer != null) && (width > 0) && (height > 0)) {
			//TODO send paint event
			LOGE("not implemented: send paint event");
		}
	}
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

void Component::setBounds(int x, int y, int width, int height) {
	synchronized (getTreeLock()) {
		boolean resized = (this->width != width) || (this->height != height);
		boolean moved = (this->x != x) || (this->y != y);
		if (!resized && !moved) return ;
		int oldX = this->x;
		int oldY = this->y;
		int oldWidth = this->width;
		int oldHeight = this->height;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		if (resized) isPacked = false;

		boolean needNotify = true;
		if (peer != null) {
			if (resized) invalidate();
			if (instanceof<Window>(this)) {
				needNotify = false;
			}
			//if (parent != null) parent->invalidateIfValid();
			invalidateParent();
		}
		if (needNotify) notifyNewBounds(resized, moved);
		repaintParentIfNeeded(oldX, oldY, oldWidth, oldHeight);
	}
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

void Window::init(const GraphicsConfiguration& gc) {
	GraphicsEnvironment::checkHeadless();
	visible = false;
	//gc = initGC(gc);
	//setLayout(BorderLayout());
	Rectangle screenBounds = gc.getBounds();
	Insets screenInsets = getToolkit().getScreenInsets(gc);
	int x = getX() + screenBounds.x + screenInsets.left;
	int y = getY() + screenBounds.y + screenInsets.top;
	if (x != this->x || y != this->y) {
		setLocation(x, y);
		//setLocationByPlatform(locationByPlatformProp);
	}
}
void Window::ownedInit(Window& owner) {
	this->parent = &owner;
}


} //namespace
