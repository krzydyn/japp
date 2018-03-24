#ifndef __AWT_COLOR_HPP
#define __AWT_COLOR_HPP

#include <lang/String.hpp>
#include <lang/Class.hpp>

namespace awt {

interface Paint : Interface {
};

class Color : extends Object, implements Paint {
private:
	int value;
	//float falpha = 0.0f;
public:
	Color() : value(0) {}
	Color(const Color& o) : value(o.value) {}
	Color& operator=(const Color& o) {
		value = o.value;
		return *this;
	}
	Color(int r, int g, int b) : Color(r, g, b, 255) {}
	Color(int r, int g, int b, int a) {
		value = ((a & 0xFF) << 24) |
                ((r & 0xFF) << 16) |
                ((g & 0xFF) << 8)  |
                ((b & 0xFF) << 0);
	}
	Color(int rgba, boolean hasalpha=false) {
		if (hasalpha) value = rgba;
		else value = 0xff000000 | rgba;
	}
	int getRed() const { return (getRGB() >> 16) & 0xFF; }
	int getGreen() const { return (getRGB() >> 8) & 0xFF; }
	int getBlue() const { return (getRGB() >> 0) & 0xFF; }
	int getAlpha() const { return (getRGB() >> 24) & 0xff; }
	int getRGB() const { return value; }
	Color brighter() const { return *this; }
	Color darker() const { return *this; }

	jint hashCode() const { return value; }
	boolean equals(const Object& obj) const {
		return instanceof<Color>(&obj) && ((const Color&)obj).getRGB() == getRGB();
	}
	String toString() const {
		return getClass().getName() + "[r=" + getRed() + ",g=" + getGreen() + ",b=" + getBlue() + "]";
	}
};

}

#endif
