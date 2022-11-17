#include <awt/Window.hpp>
#include <lang/System.hpp>

void showWindow() {
	awt::Window& w = *(new awt::Window(null));
	w.setBounds(200,100,200,150);
	w.setBackground(awt::Color::BLUE);
	w.setVisible(true);

	awt::Frame& f = *(new awt::Frame("Test title"));
	f.setBounds(300,200,300,150);
	f.setVisible(true);
	Thread::sleep(100);
}

int main() {
	showWindow();
	LOGD("main end");
}
