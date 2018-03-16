#ifndef __AWT_TOOLKIT_HPP
#define __AWT_TOOLKIT_HPP

#include <awt/peer/WindowPeer.hpp>
#include <lang/Exception.hpp>

namespace awt {

class HeadlessException : extends UnsupportedOperationException {
	using UnsupportedOperationException::UnsupportedOperationException;
};

class GraphicsEnvironment {
public:
	static boolean isHeadless() { return false; }
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
};

} //namespace

#endif
