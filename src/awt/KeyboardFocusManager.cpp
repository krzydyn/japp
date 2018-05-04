#include <awt/InputEvent.hpp>
#include <awt/KeyboardFocusManager.hpp>

namespace {
class DefaultKeyboardFocusManager : extends awt::KeyboardFocusManager {
	boolean dispatchEvent(awt::AWTEvent& e) const override {
		switch (e.getID()) {
		}
		return true;
	}
	void processKeyEvent(awt::Component* focusedComponent, awt::AWTEvent& e) const override {
		if (e.getID() == awt::KeyEvent::KEY_TYPED) return;
	}
};

Object KeyboardFocusManager_class;
awt::KeyboardFocusManager* manager = null;

awt::Component* focusOwner = null;
//awt::Component* permanentFocusOwner = null;
//awt::Window* focusedWindow = null;
//awt::Window* activeWindow = null;
}

namespace awt {

KeyboardFocusManager* KeyboardFocusManager::getCurrentKeyboardFocusManager() {
	if (manager == null) manager = new DefaultKeyboardFocusManager();
	return manager;
}

AWTEvent& KeyboardFocusManager::retargetFocusEvent(AWTEvent& event) {
	return event;
}

boolean KeyboardFocusManager::isAutoFocusTransferEnabledFor(Component *comp) {
	//return isAutoFocusTransferEnabled() && comp->isAutoFocusTransferOnDisposal();
	return false;
}

Component* KeyboardFocusManager::getFocusOwner() {
	synchronized (KeyboardFocusManager_class) {
		if (focusOwner == null) return null;
		//return (focusOwner.appContext == AppContext.getAppContext()) ? focusOwner ? null;
		return focusOwner;
	}
	return null;
}

}
