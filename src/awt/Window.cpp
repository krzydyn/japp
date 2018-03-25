#include <awt/Window.hpp>
#include <lang/System.hpp>

namespace {
ArrayList<awt::Window*> allWindows;
}

namespace awt {

Object Component::LOCK;

void Component::reshapeNativePeer(int x, int y, int width, int height, int op) {
	LOGD("Component::%s(%d,%d,%d,%d)", __FUNCTION__, x, y, width, height);
	// native peer might be offset by more than direct
	// parent since parent might be lightweight.
	int nativeX = x;
	int nativeY = y;
	for (Component* c = parent; (c != null) && (instanceof<LightweightPeer>(c->peer)); c = c->parent) {
		nativeX += c->x;
		nativeY += c->y;
	}
	peer->setBounds(nativeX, nativeY, width, height, op);
}

void Component::applyCurrentShape() {
}
void Component::applyCurrentShapeBelowMe() {
	Container* parent = getContainer();
	if (parent != null && parent->isShowing()) {
	}
}
void Component::subtractAndApplyShapeBelowMe() {
	Container* parent = getContainer();
	if (parent != null && parent->isShowing()) {
	}
}

void Component::mixOnShowing() {
	synchronized (getTreeLock()) {
		if (!isMixingNeeded()) return ;
		if (isLightweight()) subtractAndApplyShapeBelowMe();
		else applyCurrentShape();
	}
}

Color Component::getBackground() const {
	Color background = this->background;
	if (background != null) return background;
	return (parent != null) ? parent->getBackground() : Color();
}

void Component::setPreferredSize(const Dimension& preferredSize) {
	Dimension old;
	if (prefSizeSet) old = prefSize;
	prefSize = preferredSize;
	prefSizeSet = (preferredSize == null);
	//firePropertyChange("preferredSize", old, preferredSize);
}
Dimension Component::getPreferredSize() {
	Dimension dim;
	if (!(isPreferredSizeSet() || isValid())) {
		synchronized (getTreeLock()) {
			prefSize = (peer != null) ? peer->getPreferredSize() : getMinimumSize();
			dim = prefSize;
		}
	}
	return dim;
}
void Component::setMinimumSize(const Dimension& minimumSize) {
	Dimension old;
	if (minSizeSet) old = minSize;
	minSize = minimumSize;
	minSizeSet = (minimumSize == null);
	//firePropertyChange("minimumSize", old, minimumSize);
}
Dimension Component::getMinimumSize() {
	Dimension dim;
	if (!(isMinimumSizeSet() || isValid())) {
		synchronized (getTreeLock()) {
			minSize = (peer != null) ? peer->getMinimumSize() : getSize();
			dim = minSize;
		}
	}
	return dim;
}

void Component::setVisible(boolean b) {
	LOGD("Component::%s(%s)", __FUNCTION__, String::valueOf(b).cstr());
	if (visible == b) { LOGW("Already visible"); return ; }
	if (b == true) { //show
		synchronized (getTreeLock()) {
			visible = true;
			//mixOnShowing();
			if (peer != null) {
				peer->setVisible(true);
				if (instanceof<LightweightPeer>(peer)) repaint();
				// updateCursorImmediately();
			}
			else {
				LOGD("peer is null");
			}
			/*if (componentListener != null || (eventMask & AWTEvent::COMPONENT_EVENT_MASK) != 0 ||
					Toolkit::enabledOnToolkit(AWTEvent::COMPONENT_EVENT_MASK)) {
				ComponentEvent e = new ComponentEvent(this, ComponentEvent::COMPONENT_SHOWN);
				Toolkit::getEventQueue().postEvent(e);
			}*/
		}
		if (parent != null) parent->invalidate();
	}
	else { //hide
		isPacked = false;
		synchronized (getTreeLock()) {
			visible = false;
			//mixOnHiding(isLightweight());
			if (peer != null) {
				peer->setVisible(false);
				if (instanceof<LightweightPeer>(peer)) repaint();
				// updateCursorImmediately();
			}
		}
		if (parent != null) parent->invalidate();
	}
}

void Component::setGraphicsConfiguration(const GraphicsConfiguration& gc) {
	synchronized(getTreeLock()) {
		if (updateGraphicsData(gc)) {
			removeNotify();
			addNotify();
		}
	}
}

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
	LOGD("Component::%s(%d,%d,%d,%d)", __FUNCTION__, x, y, width, height);
	synchronized (getTreeLock()) {
		setBoundsOp(ComponentPeer::SET_BOUNDS);
		Finalize(setBoundsOp(ComponentPeer::RESET_OPERATION););

		boolean resized = (this->width != width) || (this->height != height);
		boolean moved = (this->x != x) || (this->y != y);
		if (!resized && !moved) {
			LOGD("  nothing changed (location or size)");
			return ;
		}
		int oldX = this->x;
		int oldY = this->y;
		int oldWidth = this->width;
		int oldHeight = this->height;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		if (resized) isPacked = false;

		LOGD("  peer = %lx", (long)peer);
		boolean needNotify = true;
		//mixOnReshaping();
		if (peer != null) {
			if (!(instanceof<LightweightPeer>(peer))) {
				 reshapeNativePeer(x, y, width, height, getBoundsOp());
			}
			if (resized) invalidate();
			if (instanceof<Window>(this)) {
				needNotify = false;
			}
			//if (parent != null) parent->invalidateIfValid();
			invalidateParent(); // the same as above
		}
		if (needNotify) notifyNewBounds(resized, moved);
		repaintParentIfNeeded(oldX, oldY, oldWidth, oldHeight);
	}
}

void Component::addNotify() {
	LOGD(__FUNCTION__);
	synchronized (getTreeLock()) {
		ComponentPeer *peer = this->peer;
		if (peer == null || instanceof<LightweightPeer>(peer)) {
			LOGD("Component::addNotify: creating peer");
			this->peer = peer = getToolkit().createComponent(this);
		}
		else {
			Container* parent = getContainer();
			if (parent != null && parent->isLightweight()) {
				//relocateComponent();
			}
		}
		invalidate();

		if (!isAddNotifyComplete) mixOnShowing();
		isAddNotifyComplete = true;
	}
}

boolean Container::updateGraphicsData(const GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	boolean ret = Component::updateGraphicsData(gc);
	//for (Component comp : component) {
	for (int i = 0; i < component.size(); i++) {
		Component* comp = component.get(i);
		if (comp != null) {
			ret |= comp->updateGraphicsData(gc);
		}
	}
	return ret;
}
Dimension Container::getPreferredSize() {
	Dimension dim;
	if (!(isPreferredSizeSet() || isValid())) {
		synchronized (getTreeLock()) {
			//prefSize = (layoutMgr != null) ? layoutMgr->getPreferredSize() : Component::getPreferredSize();
			prefSize = Component::getPreferredSize();
			dim = prefSize;
		}
	}
	return dim;
}
void Container::addNotify() {
	synchronized (getTreeLock()) {
		Component::addNotify();
		if (! (instanceof<LightweightPeer>(peer))) {
			//dispatcher = new LightweightDispatcher(this);
		}

		for (int i = 0; i < component.size(); i++) {
			component.get(i)->addNotify();
		}
	}
}

void Window::setGraphicsConfiguration(const GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	Container::setGraphicsConfiguration(gc);
}
const GraphicsConfiguration& Window::initGC(const GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	const GraphicsConfiguration* rgc = &gc;
	GraphicsEnvironment::checkHeadless();
	if (gc == null) {
		rgc = &GraphicsEnvironment::getLocalGraphicsEnvironment().
			getDefaultScreenDevice().getDefaultConfiguration();
	}
	setGraphicsConfiguration(*rgc);
	return *rgc;
}

void Window::init(const GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	GraphicsEnvironment::checkHeadless();
	visible = false;
	const GraphicsConfiguration& rgc = initGC(gc);
	//setLayout(BorderLayout());
	Rectangle screenBounds = rgc.getBounds();
	Insets screenInsets = getToolkit().getScreenInsets(rgc);
	int x = getX() + screenBounds.x + screenInsets.left;
	int y = getY() + screenBounds.y + screenInsets.top;
	if (x != this->x || y != this->y) {
		setLocation(x, y);
		//setLocationByPlatform(locationByPlatformProp);
	}
}

void Window::ownedInit(Window& owner) {
	LOGD(__FUNCTION__);
	this->parent = &owner;
}

void Window::pack() {
	if (parent != null && parent->getPeer() == null) {
		parent->addNotify();
	}
	if (peer == null) addNotify();
	Dimension newSize = getPreferredSize();
	if (peer != null) {
		setClientSize(newSize.width, newSize.height);
	}
	if (beforeFirstShow) isPacked = true;
	validateUnconditionally();
}

void Window::toFront() {
	if (visible) {
		WindowPeer* peer = (WindowPeer*)((void*)this->peer);
		if (peer != null) peer->toFront();
		//if (isModalBlocked()) modalBlocker.toFront();
	}
}
void Window::setVisible(boolean b) {
	LOGD("Window::%s(%s)", __FUNCTION__, String::valueOf(b).cstr());
	if (b) {
		if (peer == null) addNotify();
		validateUnconditionally();
		isInShow = true;
		if (visible) {
			toFront();
		}
		else {
			beforeFirstShow = false;
			//closeSplashScreen();
			Container::setVisible(true);
			synchronized (getTreeLock()) {
				locationByPlatform = false;
			}
			//for (int i = 0; i < ownedWindowList.size(); i++) { }
		}
		isInShow = false;

		if ((state & OPENED) == 0) {
			//postWindowEvent(WindowEvent.WINDOW_OPENED);
			state |= OPENED;
		}
	}
}

void Window::addNotify() {
	LOGD("Window::%s()", __FUNCTION__);
	synchronized (getTreeLock()) {
		if (parent != null && parent->getPeer() == null) {
			parent->addNotify();
		}
		if (peer == null) peer = getToolkit().createWindow(this);
		synchronized (allWindows) {
			allWindows.add(this);
		}
		Container::addNotify();
	}
}
void Window::removeNotify() {
	synchronized (getTreeLock()) {
		synchronized (allWindows) {
			allWindows.remove(this);
		}
		Container::removeNotify();
	}
}

} //namespace
