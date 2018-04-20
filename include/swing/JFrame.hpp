#ifndef __SWING_JFRAME_HPP
#define __SWING_JFRAME_HPP

#include <awt/Window.hpp>
#include <swing/WindowConstants.hpp>

namespace swing {

class JRootPane;
class JFrame : extends awt::Frame, implements WindowConstants {
private:
	int defaultCloseOperation = HIDE_ON_CLOSE;

protected:
	JRootPane *rootPane;
	boolean rootPaneCheckingEnabled = false;

	void frameInit();

public:
	static const int EXIT_ON_CLOSE = 3;

	JFrame() : awt::Frame() { frameInit(); }
	JFrame(awt::GraphicsConfiguration& gc) : awt::Frame(gc) { frameInit(); }
	JFrame(const String& title) : awt::Frame(title) { frameInit(); }
	JFrame(const String& title, awt::GraphicsConfiguration& gc) : awt::Frame(title, gc) { frameInit(); }
	
};

}

#endif
