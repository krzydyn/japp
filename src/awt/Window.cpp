#include <awt/Window.hpp>

boolean Component::isShowing() const {
	if (visible && peer != null) {
		return (parent == null) || parent->isShowing();
	}
	return false;
}
