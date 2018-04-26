#include <awt/KeyboardFocusManager.hpp>

namespace {
Object KeyboardFocusManager_class;

awt::Component* focusOwner = null;
//awt::Component* permanentFocusOwner = null;
//awt::Window* focusedWindow = null;
//awt::Window* activeWindow = null;
}

namespace awt {

KeyboardFocusManager* KeyboardFocusManager::getCurrentKeyboardFocusManager() {
	return null;
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
