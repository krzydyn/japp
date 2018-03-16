#include "XlibWrapper.hpp"
#include <X11/Xlib.h>

namespace awt { namespace x11 {

long XlibWrapper::XOpenDisplay(const String& name) {
	return (long)::XOpenDisplay(name.cstr());
}
int XlibWrapper::XDefaultScreen(long display) {
	return ::XDefaultScreen((Display *)display);
}

long XlibWrapper::XBlackPixel(long display, int screen) {
	return (long)::XBlackPixel((Display *)display, screen);
}
long XlibWrapper::XWhitePixel(long display, int screen) {
	return (long)::XWhitePixel((Display *)display, screen);
}

}}
