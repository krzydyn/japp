#include <awt/Window.hpp>

namespace awt {

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
