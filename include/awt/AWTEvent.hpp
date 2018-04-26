#ifndef __AWT_AWTEVENT_HPP
#define __AWT_AWTEVENT_HPP

#include <lang/Class.hpp>
#include <lang/String.hpp>

namespace awt {

/* obsolete
class Event : extends Object {
private:
	long data;
	boolean consumed = false;

public:
	Object* target;
	long when;
	int id;
	int x;
	int y;
	int key;
	int modifiers;
	int clickCount;
	Shared<Object> arg;

	Event *evt; //next event
};
*/
class EventObject : extends Object {
protected:
	Object*  source;
public:
	EventObject(Object* source);
	Object* getSource() { return source; }
	String toString() const override { return getClass().getName() + "[source=" + *source + "]"; }
};

class ComponentPeer;
class AWTEvent : extends EventObject {
private:
	boolean isSystemGenerated;
	void nativeSetSource(ComponentPeer* peer);

protected:
	int id;
	boolean consumed = false;

	void consume();

public:
	static const long COMPONENT_EVENT_MASK = 0x01;
	static const long CONTAINER_EVENT_MASK = 0x02;
	static const long FOCUS_EVENT_MASK = 0x04;
	static const long KEY_EVENT_MASK = 0x08;
	static const long MOUSE_EVENT_MASK = 0x10;
	static const long MOUSE_MOTION_EVENT_MASK = 0x20;
	static const long WINDOW_EVENT_MASK = 0x40;
	static const long ACTION_EVENT_MASK = 0x80;
	static const long ADJUSTMENT_EVENT_MASK = 0x100;
	static const long ITEM_EVENT_MASK = 0x200;
	static const long TEXT_EVENT_MASK = 0x400;
	static const long INPUT_METHOD_EVENT_MASK = 0x800;
	static const long INPUT_METHODS_ENABLED_MASK = 0x1000;
	static const long PAINT_EVENT_MASK = 0x2000;
	static const long INVOCATION_EVENT_MASK = 0x4000;
	static const long HIERARCHY_EVENT_MASK = 0x8000;
	static const long HIERARCHY_BOUNDS_EVENT_MASK = 0x10000;
	static const long MOUSE_WHEEL_EVENT_MASK = 0x20000;
	static const long WINDOW_STATE_EVENT_MASK = 0x40000;
	static const long WINDOW_FOCUS_EVENT_MASK = 0x80000;

	static const int RESERVED_ID_MAX = 1999;

	boolean focusManagerIsDispatching = false;
	boolean isPosted;

	//AWTEvent(const Event& event) : AWTEvent(event.target, event.id) {}
	AWTEvent(Object* source, int id);

	void setSource(Object* newSource);
	int getID() const { return id; }
	boolean isConsumed() const {return consumed;}

	String toString() const override;
	virtual String paramString() const {return "";}

	void dispatched();
};

class AdjustmentEvent : extends AWTEvent {
public:
	static const int ADJUSTMENT_FIRST    = 601;
	static const int ADJUSTMENT_LAST     = 601;
	static const int ADJUSTMENT_VALUE_CHANGED = ADJUSTMENT_FIRST;
};
class ItemEvent : extends AWTEvent {
public:
	static const int ITEM_FIRST          = 701;
	static const int ITEM_LAST           = 701;
	static const int ITEM_STATE_CHANGED  = ITEM_FIRST;
};
class TextEvent : extends AWTEvent {
public:
	static const int TEXT_FIRST  = 900;
	static const int TEXT_LAST   = 900;
	static const int TEXT_VALUE_CHANGED  = TEXT_FIRST;
};
class ActionEvent : extends AWTEvent {
private:
	String actionCommand;
	long when;
	int modifiers;
public:
	static const int ACTION_FIRST                = 1001;
	static const int ACTION_LAST                 = 1001;
	static const int ACTION_PERFORMED    = ACTION_FIRST;

	ActionEvent(Object* source, int id, const String& command) : ActionEvent(source, id, command, 0) {}
	ActionEvent(Object* source, int id, const String& command, int modifiers) : ActionEvent(source, id, command, 0, modifiers) {}
	ActionEvent(Object* source, int id, const String& command, long when, int modifiers) : AWTEvent(source, id),
			actionCommand(command), when(when), modifiers(modifiers) {}
};
class Component;
class ComponentEvent : extends AWTEvent {
public:
	static const int COMPONENT_FIRST             = 100;
	static const int COMPONENT_LAST              = 103;
	static const int COMPONENT_MOVED     = COMPONENT_FIRST;
	static const int COMPONENT_RESIZED   = 1 + COMPONENT_FIRST;
	static const int COMPONENT_SHOWN     = 2 + COMPONENT_FIRST;
	static const int COMPONENT_HIDDEN    = 3 + COMPONENT_FIRST;

	ComponentEvent(Component* source, int id) : AWTEvent((Object*)source, id) {}
	Component* getComponent();
};

class Container;
class HierarchyEvent : extends AWTEvent {
private:
	Component* changed;
	Container* changedParent;
	long      changeFlags = 0;

public:
	static const int HIERARCHY_FIRST = 1400;
	static const int HIERARCHY_CHANGED = HIERARCHY_FIRST;
	static const int ANCESTOR_MOVED = 1 + HIERARCHY_FIRST;
	static const int ANCESTOR_RESIZED = 2 + HIERARCHY_FIRST;
	static const int HIERARCHY_LAST = ANCESTOR_RESIZED;

	static const int PARENT_CHANGED = 0x1;
	static const int DISPLAYABILITY_CHANGED = 0x2;
	static const int SHOWING_CHANGED = 0x4;

	HierarchyEvent(Component* source, int id, Component* changed, Container* changedParent) : AWTEvent((Object*)source,id),
		changed(changed), changedParent(changedParent) {}
	HierarchyEvent(Component* source, int id, Component* changed, Container* changedParent, long changeFlags) :
		AWTEvent((Object*)source,id),
		changed(changed), changedParent(changedParent), changeFlags(changeFlags) {}
};

interface EventListener : Interface {
};
interface ComponentListener : extends EventListener {
public:
	virtual void componentResized(ComponentEvent& e) = 0;
	virtual void componentMoved(ComponentEvent& e) = 0;
	virtual void componentShown(ComponentEvent& e) = 0;
	virtual void componentHidden(ComponentEvent& e) = 0;
};

}

#endif
