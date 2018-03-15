#ifndef __AWT_POINT_HPP
#define __AWT_POINT_HPP

namespace awt {

class Point : extends Object {
public:
	Point(const Point& o) : x(o.x), y(o.y) {}

	int x;
	int y;
	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}
	Point getLocation() { return Point(x, y); }
};
class Dimension : extends Object {
public:
	Dimension(const Dimension& o) : width(o.width), height(o.height) {}

	int width;
	int height;	
	Dimension() : width(0), height(0) {}
	Dimension(int w, int h) : width(w), height(h) {}
};

class Rectangle : extends Object {
public:
	Rectangle(const Rectangle& o) : x(o.x), y(o.y), width(o.width), height(o.height) {}

	int x;
	int y;
	int width;
	int height;	

	Rectangle() : x(0), y(0), width(0), height(0) {}
	Rectangle(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

}

#endif
