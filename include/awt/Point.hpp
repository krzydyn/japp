#ifndef __AWT_POINT_HPP
#define __AWT_POINT_HPP

namespace awt {

class Point : extends Object {
public:
	int x;
	int y;
	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}
	Point getLocation() { return Point(x, y); }
};
class Dimension : extends Object {
public:
	int width;
	int height;	
};

}

#endif
