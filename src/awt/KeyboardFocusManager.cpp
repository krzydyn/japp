#include <awt/KeyboardFocusManager.hpp>

namespace awt {

KeyboardFocusManager* KeyboardFocusManager::getCurrentKeyboardFocusManager() {
	return null;
}

AWTEvent& KeyboardFocusManager::retargetFocusEvent(AWTEvent& event) {
	return event;
}

}
