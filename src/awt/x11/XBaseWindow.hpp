#ifndef __AWT_X11_XBASEWINDOW_HPP
#define __AWT_X11_XBASEWINDOW_HPP
 
#include <util/HashMap.hpp>

#include "XToolkit.hpp"

namespace awt {
class Component;
namespace x11 {

class XBaseWindow;
class XAwtState {
public:
	static void setComponentMouseEntered(Component* component);
	static awt::Component* getComponentMouseEntered();
	static boolean isManualGrab();
	static void setGrabWindow(XBaseWindow* grabWindow);
	static void setAutoGrabWindow(XBaseWindow* grabWindow);
	static XBaseWindow *getGrabWindow();
};

class XWMHints : extends Object {
};

class XSizeHints : extends XDataWrapper {
public:
	static int getSize() { return 80; }
	XSizeHints() : XDataWrapper(getSize()) {}
	XSizeHints(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	long get_flags() { return (pData->getLong(0)); }
	void set_flags(long v) { pData->putLong(0, v); }
	int get_x() { return (pData->getInt(8)); }
	void set_x(int v) { pData->putInt(8, v); }
	int get_y() { return (pData->getInt(12)); }
	void set_y(int v) { pData->putInt(12, v); }
	int get_width() { return (pData->getInt(16)); }
	void set_width(int v) { pData->putInt(16, v); }
	int get_height() { return (pData->getInt(20)); }
	void set_height(int v) { pData->putInt(20, v); }
	int get_min_width() { return (pData->getInt(24)); }
	void set_min_width(int v) { pData->putInt(24, v); }
	int get_min_height() { return (pData->getInt(28)); }
	void set_min_height(int v) { pData->putInt(28, v); }
	int get_max_width() { return (pData->getInt(32)); }
	void set_max_width(int v) { pData->putInt(32, v); }
	int get_max_height() { return (pData->getInt(36)); }
	void set_max_height(int v) { pData->putInt(36, v); }
	int get_width_inc() { return (pData->getInt(40)); }
	void set_width_inc(int v) { pData->putInt(40, v); }
	int get_height_inc() { return (pData->getInt(44)); }
	void set_height_inc(int v) { pData->putInt(44, v); }
	int get_min_aspect_x() { return (pData->getInt(48)); }
	void set_min_aspect_x(int v) { pData->putInt(48, v); }
	int get_min_aspect_y() { return (pData->getInt(52)); }
	void set_min_aspect_y(int v) { pData->putInt(52, v); }
	int get_max_aspect_x() { return (pData->getInt(56)); }
	void set_max_aspect_x(int v) { pData->putInt(56, v); }
	int get_max_aspect_y() { return (pData->getInt(60)); }
	void set_max_aspect_y(int v) { pData->putInt(60, v); }
	int get_base_width() { return (pData->getInt(64)); }
	void set_base_width(int v) { pData->putInt(64, v); }
	int get_base_height() { return (pData->getInt(68)); }
	void set_base_height(int v) { pData->putInt(68, v); }
	int get_win_gravity() { return (pData->getInt(72)); }
	void set_win_gravity(int v) { pData->putInt(72, v); }
};

// from XVisualInfo *XGetVisualInfo( Display* display, long vinfo_mask, XVisualInfo* vinfo_template, int* nitems_return);
class XVisualInfo : extends XDataWrapper {
public:
	static int getSize() { return 64; }
	XVisualInfo() : XDataWrapper(getSize()) {}
	XVisualInfo(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	long get_visual(int index) { return pData->getLong(0)+index*getLongSize(); }
	long get_visual() { return pData->getLong(0); }
	void set_visual(long v) { pData->putLong(0, v); }
	long get_visualid() { return (pData->getLong(8)); }
	void set_visualid(long v) { pData->putLong(8, v); }
	int get_screen() { return (pData->getInt(16)); }
	void set_screen(int v) { pData->putInt(16, v); }
	int get_depth() { return (pData->getInt(20)); }
	void set_depth(int v) { pData->putInt(20, v); }
	int get_class() { return (pData->getInt(24)); }
	void set_class(int v) { pData->putInt(24, v); }
	long get_red_mask() { return (pData->getLong(32)); }
	void set_red_mask(long v) { pData->putLong(32, v); }
	long get_green_mask() { return (pData->getLong(40)); }
	void set_green_mask(long v) { pData->putLong(40, v); }
	long get_blue_mask() { return (pData->getLong(48)); }
	void set_blue_mask(long v) { pData->putLong(48, v); }
	int get_colormap_size() { return (pData->getInt(56)); }
	void set_colormap_size(int v) { pData->putInt(56, v); }
	int get_bits_per_rgb() { return (pData->getInt(60)); }
	void set_bits_per_rgb(int v) { pData->putInt(60, v); }
};

class AwtGraphicsConfigData : extends XDataWrapper {
public:
	static int getSize() { return 208; }
	AwtGraphicsConfigData() : XDataWrapper(getSize()) {}
	AwtGraphicsConfigData(Shared<nio::ByteBuffer> buf) : XDataWrapper(buf) {}

	int get_awt_depth() { return (pData->getInt(0)); }
	void set_awt_depth(int v) { pData->putInt(0, v); }
	long get_awt_cmap() { return (pData->getLong(8)); }
	void set_awt_cmap(long v) { pData->putLong(8, v); }
	XVisualInfo get_awt_visInfo() { return XVisualInfo(nio::ByteBuffer::wrap(pData->array(), 16, 64)); }
	int get_awt_num_colors() { return (pData->getInt(80)); }
	void set_awt_num_colors(int v) { pData->putInt(80, v); }
	//awtImageData get_awtImage(int index) { return (pData->getLong(88) != 0)?(new awtImageData(Native.getLong(pData+88)+index*560)):(null); }
	long get_awtImage() { return pData->getLong(88); }
	void set_awtImage(long v) { pData->putLong(88, v); }
	long get_AwtColorMatch(int index) { return pData->getLong(96)+index*getLongSize(); }
	long get_AwtColorMatch() { return pData->getLong(96); }
	void set_AwtColorMatch(long v) { pData->putLong(96, v); }
	long get_monoImage(int index) { return pData->getLong(104)+index*getLongSize(); }
	long get_monoImage() { return pData->getLong(104); }
	void set_monoImage(long v) { pData->putLong(104, v); }
	long get_monoPixmap() { return (pData->getLong(112)); }
	void set_monoPixmap(long v) { pData->putLong(112, v); }
	int get_monoPixmapWidth() { return (pData->getInt(120)); }
	void set_monoPixmapWidth(int v) { pData->putInt(120, v); }
	int get_monoPixmapHeight() { return (pData->getInt(124)); }
	void set_monoPixmapHeight(int v) { pData->putInt(124, v); }
	long get_monoPixmapGC() { return (pData->getLong(128)); }
	void set_monoPixmapGC(long v) { pData->putLong(128, v); }
	int get_pixelStride() { return (pData->getInt(136)); }
	void set_pixelStride(int v) { pData->putInt(136, v); }
	//ColorData get_color_data(int index) { return (pData->getLong(144) != 0)?(new ColorData(Native.getLong(pData+144)+index*88)):(null); }
	long get_color_data() { return pData->getLong(144); }
	void set_color_data(long v) { pData->putLong(144, v); }
	long get_glxInfo(int index) { return pData->getLong(152)+index*getLongSize(); }
	long get_glxInfo() { return pData->getLong(152); }
	void set_glxInfo(long v) { pData->putLong(152, v); }
	int get_isTranslucencySupported() { return (pData->getInt(160)); }
	void set_isTranslucencySupported(int v) { pData->putInt(160, v); }
	//XRenderPictFormat get_renderPictFormat() { return new XRenderPictFormat(pData + 168); }
};

class XCreateWindowParams : extends Object {
private:
	util::HashMap<String,Object*> params;
public:
	XCreateWindowParams(const XCreateWindowParams& o) : params(o.params) {}
	XCreateWindowParams() {}
	~XCreateWindowParams();

	template<class T, class std::enable_if<std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	XCreateWindowParams& putIfNull(const String& key, const T& value) {
		if (value != null) {
			if (!params.containsKey(key)) params.put(key, new T(value));
		}
		return *this;
	}
	XCreateWindowParams& putIfNull(const String& key, long value) { return putIfNull(key, Long(value)); }

	template<class T, class std::enable_if<std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	XCreateWindowParams& put(const String& key, const T& value) {
		if (value != null) {
			remove(key);
			params.put(key, new T(value));
		}
		return *this;
	}
	XCreateWindowParams& put(const String& key, long value) { return put(key, Long(value)); }

	boolean containsKey(const String& key) { return params.containsKey(key); }

	template<class T, class std::enable_if<std::is_base_of<Object,T>::value,Object>::type* = nullptr>
	T& get(const String& key) const {
		if (!params.containsKey(key)) return (T&)null_obj;
		Object *o = params.get(key);
		return (T&)(*o);
	}

	XCreateWindowParams& remove(const String& key) {
		if (params.containsKey(key))
			delete params.remove(key);
		return *this;
	}
};

class XRootWindow;
class XBaseWindow : extends Object {
private:
	XCreateWindowParams delayedParams;

	long window = 0;
	boolean mapped = false;
	boolean embedded = false;
	XBaseWindow* parentWindow;

	long screen = 0;

	enum class InitialiseState {
		INITIALISING,
		NOT_INITIALISED,
		INITIALISED,
		FAILED_INITIALISATION
	};
	InitialiseState initialising = InitialiseState::NOT_INITIALISED;

protected:
	int x;
	int y;
	int width;
	int height;

	boolean visible = false;

	// Creates an invisible InputOnly window without an associated Component.
	//XBaseWindow() : XBaseWindow(XCreateWindowParams()) {}
	XBaseWindow() {} //dummy

	XBaseWindow(XCreateWindowParams& params);
	XBaseWindow(long parentWindow, const Rectangle& bounds);
	XBaseWindow(const Rectangle& bounds);
	XBaseWindow(long parentWindow);

	virtual void init(XCreateWindowParams& params) final;
	virtual void checkParams(XCreateWindowParams& params);
	virtual void preInit(XCreateWindowParams& params);
	virtual void create(XCreateWindowParams& params);
	virtual void postInit(XCreateWindowParams& params);
	virtual void ungrabInputImpl() {}
	void setSizeHints(long flags, int x, int y, int width, int height);

public:
    static const char* PARENT_WINDOW; // parent window, Long
    static const char* BOUNDS; // bounds of the window, Rectangle
    static const char* OVERRIDE_REDIRECT; // override_redirect setting, Boolean
    static const char* EVENT_MASK; // event mask, Integer
    static const char* VALUE_MASK; // value mask, Long
    static const char* BORDER_PIXEL; // border pixel value, Integer
    static const char* COLORMAP; // color map, Long
    static const char* DEPTH; // depth, Integer
    static const char* VISUAL_CLASS; // visual class, Integer
    static const char* VISUAL; // visual, Long
    static const char* EMBEDDED; // is embedded?, Boolean
    static const char* DELAYED; // is creation delayed?, Boolean
    static const char* PARENT; // parent peer
    static const char* BACKGROUND_PIXMAP; // background pixmap
    static const char* VISIBLE; // whether it is visible by default
    static const char* SAVE_UNDER; // save content under this window
    static const char* BACKING_STORE; // enables double buffering
    static const char* BIT_GRAVITY; // copy old content on geometry change

	static long getScreenOfWindow(long window);
	static void ungrabInput();
	static XRootWindow *getXAWTRootWindow();

	virtual void init() final;

	virtual long getScreenNumber();
	virtual long getScreen();
	virtual long getWindow() { return window; }
    virtual long getContentWindow() { return window; }

	virtual boolean isVisible() { return visible; }
	virtual Rectangle getBounds() { return Rectangle(x, y, width, height); }
	virtual void xSetVisible(boolean visible);
	virtual void xSetBounds(const Rectangle& bounds) {
		xSetBounds(bounds.x, bounds.y, bounds.width, bounds.height);
	}
	virtual void xSetBounds(int x, int y, int width, int height);

	static void dispatchToWindow(const XEvent& ev);
	virtual void dispatchEvent(const XEvent& ev);
};

}}

#endif
