#include <awt/InputEvent.hpp>

namespace awt {

String KeyEvent::paramString() const {
	StringBuilder str;
	switch (id) {
	case KEY_PRESSED:
		str.append("KEY_PRESSED");
		break;
	case KEY_RELEASED:
		str.append("KEY_RELEASED");
		break;
	case KEY_TYPED:
		str.append("KEY_TYPED");
		break;
	default:
		str.append("unknown type");
		break;
	}
	str.append(",keyCode=").append(keyCode);
	return str.toString();
}

}
