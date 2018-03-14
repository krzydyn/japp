#include <awt/Window.hpp>

LightweightPeer* Toolkit::createComponent(Component* target) {
	if (lightweightMarker == null) lightweightMarker = new NullComponentPeer();
	return lightweightMarker;
}

Object Component::LOCK;

boolean Component::isShowing() const {
	if (visible && peer != null) {
		return (parent == null) || parent->isShowing();
	}
	return false;
}

Toolkit& Component::getToolkit() {
	Container* parent = this->parent;
	if (parent != null) return parent->getToolkitImpl();
		return Toolkit::getDefaultToolkit();
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
