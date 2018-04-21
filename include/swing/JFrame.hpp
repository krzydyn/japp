#ifndef __SWING_JFRAME_HPP
#define __SWING_JFRAME_HPP

#include <awt/Window.hpp>
#include <swing/WindowConstants.hpp>

namespace swing {

class ComponentUI;
class JComponent : extends awt::Container {
protected:
	virtual void setUI(ComponentUI* newUI);
	virtual awt::Graphics& getComponentGraphics(awt::Graphics& g);
	virtual void paintComponent(awt::Graphics& g);
	virtual void paintChildren(awt::Graphics& g);
	virtual void paintBorder(awt::Graphics& g);

	virtual void printComponent(awt::Graphics& g) { paintComponent(g); }
	virtual void printChildren(awt::Graphics& g) { paintChildren(g); }
	virtual void printBorder(awt::Graphics& g) { paintBorder(g); }

	//boolean requestFocusInWindow(boolean temporary) override { return Container::requestFocusInWindow(temporary); }
public:
	virtual void updateUI() {}
	void update(awt::Graphics& g) override { paint(g); }
	void paint(awt::Graphics& g) override;
	void printAll(awt::Graphics& g) override;
	void print(awt::Graphics& g) override;

	void requestFocus() override { Container::requestFocus(); }
	boolean requestFocus(boolean temporary) override { return Container::requestFocus(temporary); }
	boolean requestFocusInWindow() override { return Container::requestFocusInWindow(); }
	void grabFocus() { requestFocus(); }
	boolean contains(int x, int y) override;

	//virtual void setInputMap(int condition, InputMap& map) final;
};

class JRootPane;
class JFrame : extends awt::Frame, implements WindowConstants {
private:
	int defaultCloseOperation = HIDE_ON_CLOSE;

protected:
	JRootPane *rootPane;
	boolean rootPaneCheckingEnabled = false;

	void frameInit();
	boolean isRootPaneCheckingEnabled() { return rootPaneCheckingEnabled; }
	void setRootPaneCheckingEnabled(boolean enabled) { rootPaneCheckingEnabled = enabled; }

public:
	static const int EXIT_ON_CLOSE = 3;

	JFrame() : awt::Frame() { frameInit(); }
	JFrame(awt::GraphicsConfiguration& gc) : awt::Frame(gc) { frameInit(); }
	JFrame(const String& title) : awt::Frame(title) { frameInit(); }
	JFrame(const String& title, awt::GraphicsConfiguration& gc) : awt::Frame(title, gc) { frameInit(); }

	void update(awt::Graphics& g) { paint(g); }
};

}

#endif
