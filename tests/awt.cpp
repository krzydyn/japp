#include <awt/Window.hpp>
#include <lang/System.hpp>

void showWindow() {
	awt::Window w;
	w.setBounds(0,0,100,100);
	w.setVisible(true);
}

int main() {
	showWindow();
	LOGD("main end");
}
