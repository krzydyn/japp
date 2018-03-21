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

	const GraphicsConfiguration* graphicsConfig = null;

	void repaintParentIfNeeded(int oldX, int oldY, int oldWidth, int oldHeight);
	void notifyNewBounds(boolean resized, boolean moved);

protected:
	static Object LOCK;

	ComponentPeer *peer = null;
	AppContext *appContext = null;
	Container *parent = null;
	int        x,y,width,height;
	Color      foreground;
	Color      background;
	Font       *font = null;
	//Cursor     cursor;
	//Locale     locale;

	boolean visible = false;
	boolean enabled = false;
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

	virtual void invalidateParent();
	virtual void setGraphicsConfiguration(const GraphicsConfiguration& gc);
	virtual void setBoundsOp(int op) {}

public:
	static constexpr float TOP_ALIGNMENT = 0.0f;
	static constexpr float CENTER_ALIGNMENT = 0.5f;
	static constexpr float BOTTOM_ALIGNMENT = 1.0f;
	static constexpr float LEFT_ALIGNMENT = 0.0f;
	static constexpr float RIGHT_ALIGNMENT = 1.0f;
	
	virtual boolean updateGraphicsData(const GraphicsConfiguration& gc) {
		if (graphicsConfig == &gc) return false;
		graphicsConfig = &gc;
		if (peer != null) return peer->updateGraphicsData(gc);
		return false;
	}

	virtual void invalidateIfValid() final {
		if (isValid()) invalidate();
	}

	virtual Object& getTreeLock() { return LOCK; }
	virtual Toolkit& getToolkit();

	virtual String getName() const { return name; }
	virtual void setName(const String& name) {
		this->name = name;
		nameExplicitlySet = true;
	}
	virtual Container *getParent() const { return parent; }
	virtual ComponentPeer* getPeer() { return peer; }
	virtual const GraphicsConfiguration& getGraphicsConfiguration() {
		if (graphicsConfig == null) throw NullPointerException("graphicsConfig");
		return *graphicsConfig;
	}
	virtual boolean isLightweight() { return instanceof<LightweightPeer>(peer); }

	virtual void setPreferredSize(const Dimension& preferredSize);
	virtual Dimension getPreferredSize();
	virtual boolean isPreferredSizeSet() { return prefSizeSet; }
	virtual void setMinimumSize(const Dimension& minimumSize);
	virtual Dimension getMinimumSize();
	virtual boolean isMinimumSizeSet() { return minSizeSet; }

	virtual boolean isValid() const { return (peer != null) && valid; }
	virtual boolean isDisplayable()  const{ return peer != null; }
	virtual boolean isVisible() const { return visible; }
	virtual Point getMousePosition() const {
		PointerInfo pi = MouseInfo::getPointerInfo();
		return pointRelativeToComponent(pi.getLocation());
	}
	virtual boolean isShowing() const;
	virtual boolean isEnabled() const { return enabled; }
	virtual void setEnabled(boolean b) { enabled = b; }
	virtual boolean isDoubleBuffered() const { return false; }
	virtual void enableInputMethods(boolean enable) {}
	virtual void setVisible(boolean b);
	virtual Color getForeground() const {
		return foreground;
	}

	virtual Container *getContainer() { return parent; }
	virtual Point getLocationOnScreen() const {
		if (peer != null && isShowing()) {
			Point pt;
			return pt;
		}
		throw IllegalComponentStateException("componentn not on screen");
	}

	virtual void validate() {
		synchronized (getTreeLock()) {
			ComponentPeer* peer = this->peer;
			boolean wasValid = isValid();
			if (!wasValid && peer != null) {
				peer->layout();
			}
			valid = true;
		}
	}
	virtual void invalidate() {
		synchronized (getTreeLock()) {
			valid = false;
			invalidateParent();
		}
	}
	virtual void revalidate() {
	}

	virtual Graphics& getGraphics();
	virtual Font& getFont();

	virtual void setLocation(int x, int y) {setBounds(x, y, width, height);}
	virtual Dimension getSize() { return Dimension(width, height); }
	virtual void setSize(int width, int height) {
		synchronized(getTreeLock()) {
			//setBoundsOp(ComponentPeer.SET_SIZE);
			setBounds(x, y, width, height);
		}
	}
	virtual void setBounds(int x, int y, int width, int height);
	virtual int getX() { return x; }
	virtual int getY() { return y; }
	virtual int getWidth() { return width; }
	virtual int getHeight() { return height; }

	virtual void repaint() { repaint(0, 0, 0, width, height); }
	virtual void repaint(int x, int y, int width, int height) { repaint(0, x, y, width, height); }
	virtual void repaint(long tm, int x, int y, int width, int height);

	virtual void addNotify();
	virtual void removeNotify(){}
};

class Container : extends Component {
private:
	util::ArrayList<Component*> component;
public:
	boolean updateGraphicsData(const GraphicsConfiguration& gc);
	virtual void validateUnconditionally() final {
	}
	Dimension getPreferredSize();
	void addNotify() {
		Component::addNotify();
		for (int i = 0; i < component.size(); i++) {
			component.get(i)->addNotify();
		}
	}
};

class Window : extends Container {
private:
	boolean beforeFirstShow = true;
	boolean isInShow = false;

	Window(const GraphicsConfiguration& gc) { init(gc); }
	const GraphicsConfiguration& initGC(const GraphicsConfiguration& gc);
	void init(const GraphicsConfiguration& gc);
	void ownedInit(Window& owner);
	void setClientSize(int w, int h) {
		synchronized (getTreeLock()) {
			setBoundsOp(ComponentPeer::SET_CLIENT_SIZE);
			setBounds(x, y, w, h);
		}
	}

protected:
	void setGraphicsConfiguration(const GraphicsConfiguration& gc);

public:
	enum class Type {
		NORMAL,
		UTILITY,
		POPUP
	};

	Window() {
		init((GraphicsConfiguration&)null_obj);
	}
/*
	Window(Frame& owner) : Window(owner == null ? (GraphicsConfiguration&)null_obj : owner.getGraphicsConfiguration())  {
		ownedInit(owner);
	}
*/
	Window(Window& owner) : Window(owner == null ? (GraphicsConfiguration&)Object::null_obj : owner.getGraphicsConfiguration())  {
		ownedInit(owner);
	}
	Window(Window& owner, const GraphicsConfiguration& gc) : Window(gc) {
		ownedInit(owner);
	}
	void setBounds(int x, int y, int width, int height) {
	}

	virtual void toFront();
	void setVisible(boolean b);
	void pack();
	void addNotify();
	void removeNotify() { Container::removeNotify(); }
};

class Frame : extends Window {
};

} //namespace awt

#endif
