#ifndef __AWT_TOOLKIT_HPP
#define __AWT_TOOLKIT_HPP

#include <awt/peer/WindowPeer.hpp>
#include <lang/Exception.hpp>

namespace awt {

class HeadlessException : extends UnsupportedOperationException {
	using UnsupportedOperationException::UnsupportedOperationException;
};

class GraphicsDevice : extends Object {
protected:
	GraphicsDevice() {}
public:
	virtual const GraphicsConfiguration& getDefaultConfiguration() = 0;
};

class GraphicsEnvironment : extends Object {
public:
	static boolean isHeadless() { return false; }
	static void checkHeadless() {
		if (isHeadless()) throw HeadlessException();
	}
	static GraphicsEnvironment& getLocalGraphicsEnvironment();
	virtual Array<GraphicsDevice*> getScreenDevices() = 0;
	virtual GraphicsDevice& getDefaultScreenDevice() = 0;
};

class GraphicsConfiguration : extends Object {
protected:
	GraphicsConfiguration(){}
public:
	virtual GraphicsDevice& getDevice() = 0;
	virtual Rectangle getBounds() const = 0;
};


class Toolkit : extends Object {
private:
	static LightweightPeer* lightweightMarker;
	static Toolkit* toolkit;
public:
	static Toolkit& getDefaultToolkit();
	virtual FramePeer* createFrame(Frame* target) = 0;
	virtual LightweightPeer* createComponent(Component* target) = 0;
	virtual WindowPeer* createWindow(Window* target) = 0;
	virtual DialogPeer* createDialog(Dialog* target) = 0;

	virtual MouseInfoPeer& getMouseInfoPeer() {
		throw UnsupportedOperationException("Not implemented");
	}

	virtual Insets getScreenInsets(const GraphicsConfiguration& gc) {
		GraphicsEnvironment::checkHeadless();
		if (*this != Toolkit::getDefaultToolkit()) {
			return Toolkit::getDefaultToolkit().getScreenInsets(gc);
		}
		else {
			return Insets(0, 0, 0, 0);
		}
	}
};

} //namespace

#endif
