#include <awt/Window.hpp>
#include <lang/Exception.hpp>

namespace awt {

EventObject::EventObject(Object* source) : source(source) {
	if (source == null) throw IllegalArgumentException("null source");
}

AWTEvent::AWTEvent(Object* source, int id) : EventObject(source),id(id) {
	switch(id) {
		case ActionEvent::ACTION_PERFORMED:
		case ItemEvent::ITEM_STATE_CHANGED:
		case AdjustmentEvent::ADJUSTMENT_VALUE_CHANGED:
		case TextEvent::TEXT_VALUE_CHANGED:
			consumed = true;
			break;
	}
}

String AWTEvent::toString() const {
	String srcName;
	return getClass().getName() + "[" + paramString() + "] on " + (srcName != null ? srcName : source->toString());
}

Component* ComponentEvent::getComponent() {
	return instanceof<Component>(source) ? (Component*)source : null;
}

}
