#include <awt/Window.hpp>
#include <lang/System.hpp>

void showWindow() {
	awt::Toolkit::getDefaultToolkit();
	awt::Window w;
	w.setVisible(true);
	LOGD("w.setVis");
}

int main() {
	showWindow();
	LOGD("main end");
}
