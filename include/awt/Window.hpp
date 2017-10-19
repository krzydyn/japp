namespace awt {

class Component : public Object {
	Container *parent;
	int x,y,width,height;
	Color       foreground;
	Color       background;
	Font        font;
	Cursor      cursor;
	Locale      locale;
	String      name;

	public String getName() { reurn name; }
	void setName(String name) { this->name = name; }
	Container *getParent() { return parent; }
	Container *getContainer() { return getParent(); }
};

class Container : public Component {
};

class Window : public Container {
};

} //namespace awt