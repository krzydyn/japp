#ifndef __AWT_KEYBOARDFOCUSMANAGER_HPP
#define __AWT_KEYBOARDFOCUSMANAGER_HPP

#include <awt/AWTEvent.hpp>

namespace awt {

class Component;
class Window;
class KeyboardFocusManager {
public:
	static const int FORWARD_TRAVERSAL_KEYS = 0;
	static const int BACKWARD_TRAVERSAL_KEYS = 1;
	static const int UP_CYCLE_TRAVERSAL_KEYS = 2;
	static const int DOWN_CYCLE_TRAVERSAL_KEYS = 3;

	static KeyboardFocusManager* getCurrentKeyboardFocusManager();
	static void setCurrentKeyboardFocusManager(KeyboardFocusManager* newManager);
	static boolean isAutoFocusTransferEnabledFor(Component *comp);

	static AWTEvent& retargetFocusEvent(AWTEvent& event);

	virtual boolean dispatchEvent(AWTEvent& e) const = 0;
	virtual boolean processKeyEvent(Component* focusedComponent, AWTEvent& e) const = 0;

	virtual Component* getFocusOwner();
};

}

#endif
