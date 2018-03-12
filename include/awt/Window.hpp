#include <awt/Color.hpp>
#include <awt/MouseInfo.hpp>
#include <lang/Exception.hpp>

namespace awt {

class IllegalComponentStateException : extends IllegalStateException {
	using IllegalStateException::IllegalStateException;
};

class ComponentPeer;
class AppContext;
class Container;
class Component : extends Object {
private:
	String  name;
	boolean nameExplicitlySet = false;
	boolean valid = false;
	boolean focusable = true;

protected:
	ComponentPeer *peer;
	AppContext *appContext;
	Container *parent;
	int        x,y,width,height;
	Color      foreground;
	Color      background;
	//Font       font;
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

	Component() {
		//appContext = AppContext.getAppContext();
	}

	virtual String constructComponentName() const {return "";}

	virtual Point pointRelativeToComponent(Point absolute) const {
		Point p0 = getLocationOnScreen();
		return Point(absolute.x - p0.x, absolute.y - p0.y);
	}

public:
	static constexpr float TOP_ALIGNMENT = 0.0f;
	static constexpr float CENTER_ALIGNMENT = 0.5f;
	static constexpr float BOTTOM_ALIGNMENT = 1.0f;
	static constexpr float LEFT_ALIGNMENT = 0.0f;
	static constexpr float RIGHT_ALIGNMENT = 1.0f;
	

	String getName() const { return name; }
	void setName(const String& name) {
		this->name = name;
		nameExplicitlySet = true;
	}
	Container *getParent() const { return parent; }
	//const GraphicsConfiguration& getGraphicsConfiguration() { }
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

	Point getLocationOnScreen() const {
		if (peer != null && isShowing()) {
			Point pt;
			return pt;
		}
		throw IllegalComponentStateException("componentn not on screen");
	}
};

class Container : extends Component {
};

class Window : extends Container {
};

} //namespace awt
