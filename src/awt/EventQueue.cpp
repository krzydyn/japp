#include <awt/EventQueue.hpp>

namespace awt {

void EventQueue::postEventPrivate(AWTEvent& theEvent) {
	theEvent.isPosted = true;
}

}
