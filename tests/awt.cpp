#include <awt/Window.hpp>
#include <lang/System.hpp>

void showWindow() {
	awt::Window w(null);
	w.setBounds(100,100,200,150);
	w.setBackground(awt::Color::BLUE);
	w.setVisible(true);
}

int main() {
	showWindow();
	LOGD("main end");
}
