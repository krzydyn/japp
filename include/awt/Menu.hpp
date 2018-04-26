#include <awt/Window.hpp>
#include <accessibility/Accessible.hpp>

namespace awt {

using namespace accessibility;

class MenuContainer;
class MenuComponent;
class MenuShortcut;
class Event;

interface MenuContainer : Interface {
public:
	//Font getFont();
	virtual void remove(MenuComponent* comp) = 0;
	virtual boolean postEvent(Event& evt) = 0;
};

class MenuComponent : extends Object {
private:
	MenuContainer* parent;
protected:
	virtual Object& getTreeLock() final;
public:
	MenuComponent() {}
	MenuContainer* getParent() { return parent; }
	void removeNotify();
};

class MenuItem : extends MenuComponent, implements Accessible {
private:
	String label;
	MenuShortcut* shortcut = null;
public:
	MenuItem() : MenuItem("", null) {}
	MenuItem(const String& label) : MenuItem(label, null) {}
	MenuItem(const String& label, MenuShortcut* s) : label(label), shortcut(s) {}
};

class Menu : extends MenuItem, implements MenuContainer, implements Accessible {
private:
	boolean tearOff;
	ArrayList<MenuComponent*> items;
protected:
	virtual int countItemsImpl() const final { return items.size(); }
public:
	Menu() : Menu("", false) {}
	Menu(const String& label) : Menu(label, false) {}
	Menu(const String& label, boolean tearOff) : MenuItem(label), tearOff(tearOff) {}

	boolean isTearOff() const { return tearOff; }
	int getItemCount() const { return countItemsImpl(); }
};

class PopupMenu : extends Menu {
private:
	boolean isTrayIconPopup = false;
public:
	PopupMenu() : PopupMenu("") {}
	PopupMenu(const String& label) : Menu(label) {}

	MenuContainer* getParent() {
		if (isTrayIconPopup) return null;
		return Menu::getParent();
	}
	void addNotify();
	void show(Component origin, int x, int y) {}
	AccessibleContext* getAccessibleContext() {return null;}
};

}
