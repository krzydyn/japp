#ifndef __AWT_GRAPHICS_HPP
#define __AWT_GRAPHICS_HPP

//#include <awt/Font.hpp>

namespace awt {

class Font : extends Object {
};

class Graphics : extends Object {
protected:
	Graphics() {}
public:
	virtual Shared<Graphics> create() = 0;
	virtual Shared<Graphics> create(int x, int y, int width, int height) {
		Shared<Graphics> g = create();
		if (g == null) return null;
		g->translate(x, y);
		g->clipRect(0, 0, width, height);
		return g;
	}

	virtual void translate(int x, int y) = 0;
	virtual Color getColor() = 0;
	virtual void setColor(Color c) = 0;
	virtual const Font& getFont() = 0;
	virtual void setFont(const Font& font) = 0;
	virtual void clipRect(int x, int y, int width, int height) = 0;
	virtual void setClip(int x, int y, int width, int height) = 0;

	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

	virtual void fillRect(int x, int y, int width, int height) = 0;
	void drawRect(int x, int y, int width, int height) {
		if ((width < 0) || (height < 0)) return ;
		if (height == 0 || width == 0) {
			drawLine(x, y, x + width, y + height);
		}
		else {
			drawLine(x, y, x + width - 1, y);
			drawLine(x + width, y, x + width, y + height - 1);
			drawLine(x + width, y + height, x + 1, y + height);
			drawLine(x, y + height, x, y + 1);
		}
	}
	virtual void clearRect(int x, int y, int width, int height) = 0;

	virtual void drawOval(int x, int y, int width, int height) = 0;
	virtual void fillOval(int x, int y, int width, int height) = 0;

	virtual void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) = 0;
	virtual void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle) = 0;

	virtual void drawString(String str, int x, int y) = 0;
};

} //namespace

#endif
