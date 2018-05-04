#ifndef __AWT_EVENTQUEUE_HPP
#define __AWT_EVENTQUEUE_HPP

#include <awt/AWTEvent.hpp>
#include <lang/Object.hpp>

namespace awt {

class EventQueue : extends Object {
private:
	void postEventPrivate(AWTEvent& theEvent);
public:
	virtual void postEvent(AWTEvent& theEvent) { postEventPrivate(theEvent); }
};

}

#endif
