#include <awt/Window.hpp>

void showWindow() {
	awt::Toolkit::getDefaultToolkit();
	awt::Window w;
	w.setVisible(true);
}

int main() {
	showWindow();
}
