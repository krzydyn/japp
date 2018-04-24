#include <awt/AWTEvent.hpp>
#include <awt/InputEvent.hpp>
#include <awt/KeyboardFocusManager.hpp>
#include <awt/Window.hpp>
#include <lang/System.hpp>

namespace {
ArrayList<awt::Window*> allWindows;

class SunDropTargetEvent : extends awt::MouseEvent {
public:
	void dispatch(){}
};
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

boolean Component::eventTypeEnabled(int type) {
	switch(type) {
		case ComponentEvent::COMPONENT_MOVED:
		case ComponentEvent::COMPONENT_RESIZED:
		case ComponentEvent::COMPONENT_SHOWN:
		case ComponentEvent::COMPONENT_HIDDEN:
			if ((eventMask & AWTEvent::COMPONENT_EVENT_MASK) != 0 || componentListener != null) {
				return true;
			}
			break;
	}
	//if (type > AWTEvent::RESERVED_ID_MAX) return true;
	return false;
}
boolean Component::areInputMethodsEnabled() {
	return ((eventMask & AWTEvent::INPUT_METHODS_ENABLED_MASK) != 0) &&
		((eventMask & AWTEvent::KEY_EVENT_MASK) != 0 || keyListener != null);
}
boolean Component::dispatchMouseWheelToAncestor(MouseWheelEvent& e) {
	synchronized (getTreeLock()) {
		Container* anc = getParent();
		if (anc != null && anc->eventEnabled(e)) {
			//anc.dispatchEventToSelf(newMWE);
			return true;
		}
	}
	return false;
}
void Component::processEvent(AWTEvent& e) {
}
void Component::dispatchEventImpl(AWTEvent& e) {
	LOGD("Component::%s(%s)", __FUNCTION__, e.toString().cstr());
	int id = e.getID();

	//0. Set timestamp and modifiers of current event.
	//EventQueue.setCurrentEventAndMostRecentTime(e);

	//1. Pre-dispatchers. Do any necessary retargeting/reordering
	if (instanceof<SunDropTargetEvent>(&e)) {
		((SunDropTargetEvent&)e).dispatch();
		return;
	}

	if (!e.focusManagerIsDispatching) {
		if (e.isPosted) {
			e = KeyboardFocusManager::retargetFocusEvent(e);
			e.isPosted = true;
		}
	}

	if (KeyboardFocusManager::getCurrentKeyboardFocusManager()->dispatchEvent(e)) return;

	if (id == MouseEvent::MOUSE_WHEEL && (!eventTypeEnabled(id)) &&
			(peer != null && !peer->handlesWheelScrolling()) &&
			(dispatchMouseWheelToAncestor((MouseWheelEvent&)e)))
		return;

	//2. Allow the Toolkit to pass this to AWTEventListeners.
	Toolkit& toolkit = Toolkit::getDefaultToolkit();
	toolkit.notifyAWTEventListeners(e);

	//3. If no one has consumed a key event, pass to KeyboardFocusManager
	if (!e.isConsumed()) {
		if (instanceof<KeyEvent>(&e)) {
			KeyboardFocusManager::getCurrentKeyboardFocusManager()->processKeyEvent(this, (KeyEvent&)e);
			if (e.isConsumed()) return;
		}
	}

	//4. Allow input methods to process the event
	if (areInputMethodsEnabled()) {
		//TODO
	}

	//5. Pre-process any special events before delivery
	switch(id) {
		case KeyEvent::KEY_PRESSED:
		case KeyEvent::KEY_RELEASED:
			break;

		case WindowEvent::WINDOW_CLOSING:
			break;

		default: break;
	}

	//6. Deliver event for normal processing
	if (newEventsOnly) {
		if (eventEnabled(e)) processEvent(e);
	}

	//8. Special handling for dialogs
	if (id == WindowEvent::WINDOW_CLOSING && !e.isConsumed()) {
	}

	//9. Allow the peer to process the event.
	if (!(instanceof<KeyEvent>(&e))) {
	}


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
				LOGD("peer is %p", peer);
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

void Component::setGraphicsConfiguration(GraphicsConfiguration& gc) {
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
		else {
			LOGD("Component::%s  peer = %lx", __FUNCTION__, (long)peer);
		}
		if (needNotify) notifyNewBounds(resized, moved);
		repaintParentIfNeeded(oldX, oldY, oldWidth, oldHeight);
	}
}

void Component::addNotify() {
	LOGD("Component::%s", __FUNCTION__);
	synchronized (getTreeLock()) {
		ComponentPeer *peer = this->peer;
		if (peer == null || instanceof<LightweightPeer>(peer)) {
			LOGD("Component::addNotify: creating peer");
			this->peer = peer = getToolkit().createComponent(this);

			if (parent != null) {
			}
		}
		else {
			Container* parent = getContainer();
			if (parent != null && parent->isLightweight()) {
				//relocateComponent();
				//if (!parent->isRecursivelyVisibleUpToHeavyweightContainer())
				//	peer->setVisible(false);
			}
		}
		invalidate();

		int npopups = popups.size();
		for (int i = 0 ; i < npopups ; i++) {
			//TODO
			//PopupMenu *popup = popups.elementAt(i);
			//popup->addNotify();
		}

		if (getFont() != null)
			peerFont = &getFont();

		if (getContainer() != null && !isAddNotifyComplete) {
			getContainer()->increaseComponentCount(this);
		}

		updateZOrder();

		if (!isAddNotifyComplete) mixOnShowing();

		isAddNotifyComplete = true;
	}
}

boolean Container::updateGraphicsData(GraphicsConfiguration& gc) {
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

void Window::setGraphicsConfiguration(GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	Container::setGraphicsConfiguration(gc);
}
GraphicsConfiguration& Window::initGC(GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	GraphicsConfiguration* rgc = &gc;
	GraphicsEnvironment::checkHeadless();
	if (gc == null) {
		rgc = &GraphicsEnvironment::getLocalGraphicsEnvironment().
			getDefaultScreenDevice().getDefaultConfiguration();
	}
	setGraphicsConfiguration(*rgc);
	return *rgc;
}

void Window::init(GraphicsConfiguration& gc) {
	LOGD(__FUNCTION__);
	GraphicsEnvironment::checkHeadless();
	visible = false;
	GraphicsConfiguration& rgc = initGC(gc);
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
		if (peer == null) {
			LOGD("Window::%s: creating peer", __FUNCTION__);
			peer = getToolkit().createWindow(this);
		}
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

void Frame::addNotify() {
	synchronized (getTreeLock()) {
		if (peer == null) peer = getToolkit().createFrame(this);
		//FramePeer* p = (FramePeer*)peer;
		Window::addNotify();
	}
}
void Frame::init(const String& title, GraphicsConfiguration& gc) {
	this->title = title;
}
void Frame::setTitle(const String& title) {
	synchronized(this) {
		this->title = title;
		FramePeer* peer = (FramePeer*)((void*)this->peer);
		peer->setTitle(title);
	}
}
void Frame::setUndecorated(boolean undecorated) {
	synchronized (getTreeLock()) {
		if (isDisplayable()) throw IllegalComponentStateException("The frame is displayable.");
	}
}

} //namespace
