#include <awt/Menu.hpp>

namespace awt {

Object& MenuComponent::getTreeLock() {
	return Component::getLOCK();
}

void MenuComponent::removeNotify() {
	synchronized (getTreeLock()) {
	}
}

}
