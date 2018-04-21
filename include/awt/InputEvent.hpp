#ifndef __AWT_INPUTEVENT_HPP
#define __AWT_INPUTEVENT_HPP

#include <awt/AWTEvent.hpp>

namespace awt {

class InputEvent : extends ComponentEvent {
private:
	long when;
	int modifiers;

protected:
	InputEvent(Component* source, int id, long when, int modifiers) : ComponentEvent(source, id), when(when), modifiers(modifiers){}
};
class WindowEvent : extends ComponentEvent {
public:
	static const int WINDOW_FIRST        = 200;
	static const int WINDOW_OPENED       = WINDOW_FIRST;
	static const int WINDOW_CLOSING      = 1 + WINDOW_FIRST;
	static const int WINDOW_CLOSED       = 2 + WINDOW_FIRST;
	static const int WINDOW_ICONIFIED    = 3 + WINDOW_FIRST;
	static const int WINDOW_DEICONIFIED  = 4 + WINDOW_FIRST;
	static const int WINDOW_ACTIVATED    = 5 + WINDOW_FIRST;
	static const int WINDOW_DEACTIVATED  = 6 + WINDOW_FIRST;
	static const int WINDOW_GAINED_FOCUS = 7 + WINDOW_FIRST;
	static const int WINDOW_LOST_FOCUS   = 8 + WINDOW_FIRST;
	static const int WINDOW_STATE_CHANGED = 9 + WINDOW_FIRST;
	static const int WINDOW_LAST         = WINDOW_STATE_CHANGED;
};
class KeyEvent : extends InputEvent {
private:
	boolean isProxyActive = false;

	int  keyCode;
	jchar keyChar;
	int keyLocation;
	long extendedKeyCode = 0;

	KeyEvent(Component* source, int id, long when, int modifiers, int keyCode, char keyChar, int keyLocation, boolean isProxyActive) :
			KeyEvent(source, id, when, modifiers, keyCode, keyChar, keyLocation) {
		this->isProxyActive = isProxyActive;
	}
	void setNewModifiers();
	void setOldModifiers();

public:
	static const int KEY_FIRST = 400;
	static const int KEY_LAST  = 402;

	static const int KEY_TYPED = KEY_FIRST;
	static const int KEY_PRESSED = 1 + KEY_FIRST;
	static const int KEY_RELEASED = 2 + KEY_FIRST;

	static const int VK_ENTER          = '\n';
	static const int VK_BACK_SPACE     = '\b';
	static const int VK_TAB            = '\t';
	static const int VK_CANCEL         = 0x03;
	static const int VK_CLEAR          = 0x0C;
	static const int VK_SHIFT          = 0x10;
	static const int VK_CONTROL        = 0x11;
	static const int VK_ALT            = 0x12;
	static const int VK_PAUSE          = 0x13;
	static const int VK_CAPS_LOCK      = 0x14;
	static const int VK_ESCAPE         = 0x1B;
	static const int VK_SPACE          = 0x20;
	static const int VK_PAGE_UP        = 0x21;
	static const int VK_PAGE_DOWN      = 0x22;
	static const int VK_END            = 0x23;
	static const int VK_HOME           = 0x24;

	static const int VK_LEFT           = 0x25;
	static const int VK_UP             = 0x26;
	static const int VK_RIGHT          = 0x27;
	static const int VK_DOWN           = 0x28;
	static const int VK_COMMA          = 0x2C;
	static const int VK_MINUS          = 0x2D;
	static const int VK_PERIOD         = 0x2E;
	static const int VK_SLASH          = 0x2F;

	static const int VK_0              = 0x30;
	static const int VK_1              = 0x31;
	static const int VK_2              = 0x32;
	static const int VK_3              = 0x33;
	static const int VK_4              = 0x34;
	static const int VK_5              = 0x35;
	static const int VK_6              = 0x36;
	static const int VK_7              = 0x37;
	static const int VK_8              = 0x38;
	static const int VK_9              = 0x39;

	static const int VK_SEMICOLON      = 0x3B;
	static const int VK_EQUALS         = 0x3D;

	static const int VK_A              = 0x41;
	static const int VK_B              = 0x42;
	static const int VK_C              = 0x43;
	static const int VK_D              = 0x44;
	static const int VK_E              = 0x45;
	static const int VK_F              = 0x46;
	static const int VK_G              = 0x47;
	static const int VK_H              = 0x48;
	static const int VK_I              = 0x49;
	static const int VK_J              = 0x4A;
	static const int VK_K              = 0x4B;
	static const int VK_L              = 0x4C;
	static const int VK_M              = 0x4D;
	static const int VK_N              = 0x4E;
	static const int VK_O              = 0x4F;
	static const int VK_P              = 0x50;
	static const int VK_Q              = 0x51;
	static const int VK_R              = 0x52;
	static const int VK_S              = 0x53;
	static const int VK_T              = 0x54;
	static const int VK_U              = 0x55;
	static const int VK_V              = 0x56;
	static const int VK_W              = 0x57;
	static const int VK_X              = 0x58;
	static const int VK_Y              = 0x59;
	static const int VK_Z              = 0x5A;

	static const int VK_OPEN_BRACKET   = 0x5B;
	static const int VK_BACK_SLASH     = 0x5C;
	static const int VK_CLOSE_BRACKET  = 0x5D;

	static const int VK_NUMPAD0        = 0x60;
	static const int VK_NUMPAD1        = 0x61;
	static const int VK_NUMPAD2        = 0x62;
	static const int VK_NUMPAD3        = 0x63;
	static const int VK_NUMPAD4        = 0x64;
	static const int VK_NUMPAD5        = 0x65;
	static const int VK_NUMPAD6        = 0x66;
	static const int VK_NUMPAD7        = 0x67;
	static const int VK_NUMPAD8        = 0x68;
	static const int VK_NUMPAD9        = 0x69;
	static const int VK_MULTIPLY       = 0x6A;
	static const int VK_ADD            = 0x6B;

	static const int VK_SEPARATOR      = 0x6C;
	static const int VK_SUBTRACT       = 0x6D;
	static const int VK_DECIMAL        = 0x6E;
	static const int VK_DIVIDE         = 0x6F;
	static const int VK_DELETE         = 0x7F; /* ASCII DEL */
	static const int VK_NUM_LOCK       = 0x90;
	static const int VK_SCROLL_LOCK    = 0x91;

	static const int VK_F1             = 0x70;
	static const int VK_F2             = 0x71;
	static const int VK_F3             = 0x72;
	static const int VK_F4             = 0x73;
	static const int VK_F5             = 0x74;
	static const int VK_F6             = 0x75;
	static const int VK_F7             = 0x76;
	static const int VK_F8             = 0x77;
	static const int VK_F9             = 0x78;
	static const int VK_F10            = 0x79;
	static const int VK_F11            = 0x7A;
	static const int VK_F12            = 0x7B;
	static const int VK_F13            = 0xF000;
	static const int VK_F14            = 0xF001;
	static const int VK_F15            = 0xF002;
	static const int VK_F16            = 0xF003;
	static const int VK_F17            = 0xF004;
	static const int VK_F18            = 0xF005;
	static const int VK_F19            = 0xF006;
	static const int VK_F20            = 0xF007;
	static const int VK_F21            = 0xF008;
	static const int VK_F22            = 0xF009;
	static const int VK_F23            = 0xF00A;
	static const int VK_F24            = 0xF00B;
	//TODO rest of VK_xx

	static const jchar CHAR_UNDEFINED   = 0xFFFF;

	static const int KEY_LOCATION_UNKNOWN  = 0;
	static const int KEY_LOCATION_STANDARD = 1;
	static const int KEY_LOCATION_LEFT     = 2;
	static const int KEY_LOCATION_RIGHT    = 3;
	static const int KEY_LOCATION_NUMPAD   = 4;

	static String getKeyText(int keyCode);
	static String getKeyModifiersText(int modifiers);
	static int getExtendedKeyCodeForChar(int c);

	KeyEvent(Component* source, int id, long when, int modifiers, int keyCode, char keyChar, int keyLocation) :
			InputEvent(source, id, when, modifiers) {
		this->keyCode = keyCode;
		this->keyChar = keyChar;
		this->keyLocation = keyLocation;
	}
	KeyEvent(Component* source, int id, long when, int modifiers, int keyCode, char keyChar) :
			KeyEvent(source, id, when, modifiers, keyCode, keyChar, KEY_LOCATION_UNKNOWN) {}

	int getKeyCode() const { return keyCode; }
	void setKeyCode(int keyCode) { this->keyCode = keyCode; }

	jchar getKeyChar() const { return keyChar; }
	void setKeyChar(jchar keyChar) { this->keyChar = keyChar; }

	int getKeyLocation() const { return keyLocation; }

	boolean isActionKey() const;
	String paramString() const override;
	int getExtendedKeyCode() { return (int)extendedKeyCode; }

};

class MouseEvent : extends InputEvent {
private:
	static int cachedNumberOfButtons;

	int x,y;
	int clickCount;
	int button;
public:
	static const int MOUSE_FIRST         = 500;
	static const int MOUSE_LAST          = 507;
	static const int MOUSE_CLICKED = MOUSE_FIRST;
	static const int MOUSE_PRESSED = 1 + MOUSE_FIRST;
	static const int MOUSE_RELEASED = 2 + MOUSE_FIRST;
	static const int MOUSE_MOVED = 3 + MOUSE_FIRST;
	static const int MOUSE_ENTERED = 4 + MOUSE_FIRST;
	static const int MOUSE_EXITED = 5 + MOUSE_FIRST;
	static const int MOUSE_DRAGGED = 6 + MOUSE_FIRST;
	static const int MOUSE_WHEEL = 7 + MOUSE_FIRST;
};
class MouseWheelEvent : extends MouseEvent {
public:
	static const int WHEEL_UNIT_SCROLL = 0;
	static const int WHEEL_BLOCK_SCROLL = 1;
};

}
#endif
