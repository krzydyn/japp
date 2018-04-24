#include <awt/Window.hpp>
#include <lang/System.hpp>

void showWindow() {
	awt::Window w(null);
	w.setBounds(200,100,200,150);
	w.setBackground(awt::Color::BLUE);
	w.setVisible(true);

	awt::Frame f("Test title");
	f.setBounds(300,200,300,150);
	f.setVisible(true);
}

int main() {
	showWindow();
	LOGD("main end");
}
