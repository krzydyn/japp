#ifndef __AWT_MOUSEINFO_HPP
#define __AWT_MOUSEINFO_HPP

#include <awt/Point.hpp>

namespace awt {

class PointerInfo {
private:
	Point location;
public:
	Point getLocation() { return location; }
};

class MouseInfo {
private:
	MouseInfo() {}
public:
	static PointerInfo getPointerInfo();
	static int getNumberOfButtons();
};

}

#endif
