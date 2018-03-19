#ifndef __AWT_WINDOW_HPP
#define __AWT_WINDOW_HPP

#include <awt/Color.hpp>
#include <awt/Graphics.hpp>
#include <awt/MouseInfo.hpp>
#include <awt/Toolkit.hpp>
#include <util/ArrayList.hpp>

namespace awt {

class IllegalComponentStateException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class AppContext;
class Container;
class Component : extends Object {
private:
	String  name;
	boolean nameExplicitlySet = false;
	boolean valid = false;
	boolean focusable = true;

protected:
	static Object LOCK;

	ComponentPeer *peer;
	AppContext *appContext;
	Container *parent;
	int        x,y,width,height;
	Color      foreground;
	Color      background;
	Font       *font;
	//Cursor     cursor;
	//Locale     locale;

	boolean visible = true;
	boolean enabled = true;
	boolean isPacked = false;

	Dimension minSize;
	boolean minSizeSet;
	Dimension prefSize;
	boolean prefSizeSet;
	Dimension maxSize;
	boolean maxSizeSet;

	Component() : peer(null) {
		//appContext = AppContext.getAppContext();
	}

	virtual String constructComponentName() const {return "";}

	virtual Point pointRelativeToComponent(Point absolute) const {
		Point p0 = getLocationOnScreen();
		return Point(absolute.x - p0.x, absolute.y - p0.y);
	}

	void invalidateParent();

	void invalidateIfValid() {
		if (isValid()) invalidate();
	}

public:
	static constexpr float TOP_ALIGNMENT = 0.0f;
	static constexpr float CENTER_ALIGNMENT = 0.5f;
	static constexpr float BOTTOM_ALIGNMENT = 1.0f;
	static constexpr float LEFT_ALIGNMENT = 0.0f;
	static constexpr float RIGHT_ALIGNMENT = 1.0f;
	

	Object& getTreeLock() { return LOCK; }
	Toolkit& getToolkit();

	String getName() const { return name; }
	void setName(const String& name) {
		this->name = name;
		nameExplicitlySet = true;
	}
	Container *getParent() const { return parent; }
	//const GraphicsConfiguration& getGraphicsConfiguration() { }
	boolean isLightweight() { return instanceof<LightweightPeer>(peer); }
	boolean isValid() const { return (peer != null) && valid; }
	boolean isDisplayable()  const{ return peer != null; }
	boolean isVisible() const { return visible; }
	Point getMousePosition() const {
		PointerInfo pi = MouseInfo::getPointerInfo();
		return pointRelativeToComponent(pi.getLocation());
	}
	boolean isShowing() const;
	boolean isEnabled() const { return enabled; }
	void setEnabled(boolean b) { enabled = b; }
	boolean isDoubleBuffered() const { return false; }
	void enableInputMethods(boolean enable) {}
	void setVisible(boolean b) {
		visible = b;
	}
	Color getForeground() const {
		return foreground;
	}

	Container *getContainer() { return parent; }
	Point getLocationOnScreen() const {
		if (peer != null && isShowing()) {
			Point pt;
			return pt;
		}
		throw IllegalComponentStateException("componentn not on screen");
	}

	void validate() {
		synchronized (getTreeLock()) {
			ComponentPeer* peer = this->peer;
			boolean wasValid = isValid();
			if (!wasValid && peer != null) {
				peer->layout();
			}
			valid = true;
		}
	}
	void invalidate() {
		synchronized (getTreeLock()) {
			valid = false;
			invalidateParent();
		}
	}
	void revalidate() {
	}

	Graphics& getGraphics();
	Font& getFont();

	virtual void addNotify();
	virtual void removeNotify() {
		synchronized (getTreeLock()) {
		}
	}
};

class Container : extends Component {
private:
	util::ArrayList<Component*> component;
public:
	void addNotify() {
		Component::addNotify();
		for (int i = 0; i < component.size(); i++) {
			component.get(i)->addNotify();
		}
	}
};

class Window : extends Container {
public:
	enum class Type {
		NORMAL,
		UTILITY,
		POPUP
	};
	void addNotify() {
		if (peer == null) peer = getToolkit().createWindow(this);
		Container::addNotify();
	}
};

class Frame : extends Window {
};

} //namespace awt

#endif
