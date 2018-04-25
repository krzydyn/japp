#include <lang/System.hpp>

#include "XlibWrapper.hpp"

#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace awt { namespace x11 {

long XlibWrapper::XOpenDisplay(const String& name) {
	long r = (long)::XOpenDisplay(name.cstr());
	LOGD("XlibWrapper::%s('%s') = %lX",__FUNCTION__, name.cstr(), r);
	return r;
}
void XlibWrapper::XCloseDisplay(long display) {
	::XCloseDisplay((Display *)display);
}
int XlibWrapper::XConnectionNumber(long display) {
	return ::XConnectionNumber((Display *)display);
}
int XlibWrapper::XScreenCount(long display) {
	return ::XScreenCount((Display *)display);
}
//long XlibWrapper::XDisplayString(long display);
//void XlibWrapper::XSetCloseDownMode(long display, int close_mode);

int XlibWrapper::XDefaultScreen(long display) {
	int r = ::XDefaultScreen((Display *)display);
	LOGD("XlibWrapper::%s(%lX) = %d", __FUNCTION__, display, r);
	return r;
}

//long XlibWrapper::ScreenOfDisplay(long display, long screen_number);
//int XlibWrapper::DoesBackingStore(long screen);
//long XlibWrapper::DisplayWidth(long display, long screen);
//long XlibWrapper::DisplayWidthMM(long display, long screen);
//long XlibWrapper::DisplayHeight(long display, long screen);
//long XlibWrapper::DisplayHeightMM(long display, long screen);
long XlibWrapper::XRootWindow(long display, long screen) {
	long r = ::XRootWindow((Display *)display, (int)screen);
	LOGD("XlibWrapper::%s(%lX,%ld) = %d", __FUNCTION__, display, screen, r);
	return r;
}
//int XlibWrapper::ScreenCount(long display);

long XlibWrapper::XBlackPixel(long display, long screen) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return (long)::XBlackPixel((Display *)display, (int)screen);
}
long XlibWrapper::XWhitePixel(long display, long screen) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return (long)::XWhitePixel((Display *)display, (int)screen);
}

long XlibWrapper::XCreateWindow(long display, long parent, int x,int  y, int width, int height, int border_width, int depth, long wclass, long visual, long valuemask, long attributes) {
	XSetWindowAttributes* a = (XSetWindowAttributes*)attributes;
	//int s = 112;
	//char buf[2*s+1];
	//for (int i=0; i < s; ++i) sprintf(buf+2*i, "%02X", ((char*)attributes)[i]);
	long r = ::XCreateWindow((Display *)display, parent, x, y, width, height, border_width, depth, (int)wclass, (Visual*)visual, valuemask, (XSetWindowAttributes*)attributes);
	//LOGD("XlibWrapper::%s(%lX,%ld,bounds=(%d,%d,%d,%d),depth=%d,wclass=%ld,vis=%ld,mask=%lX,attrs=%s) = %lX", __FUNCTION__, display, parent, x, y, width, height, depth, wclass, visual, valuemask, buf, r);
	LOGD("XlibWrapper::%s(%lX,%ld,bounds=(%d,%d,%d,%d),depth=%d,wclass=%ld,vis=%ld,mask=%lX,attrs=(wg=%d,em=%lX)) = %lX", __FUNCTION__, display, parent, x, y, width, height, depth, wclass, visual, valuemask, a->win_gravity, a->event_mask, r);
	return r;
}
void XlibWrapper::XDestroyWindow(long display, long window) {
	::XDestroyWindow((Display *)display, window);
}

//int XlibWrapper::XGrabPointer(long display, long grab_window, int owner_events, int event_mask, int pointer_mode, int keyboard_mode, long confine_to, long cursor, long time);

void XlibWrapper::XUngrabPointer(long display, long time) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XUngrabPointer((Display *)display, time);
}

//int XlibWrapper::XGrabKeyboard(long display, long grab_window, int owner_events, int pointer_mode, int keyboard_mode, long time);

void XlibWrapper::XUngrabKeyboard(long display, long time) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XUngrabKeyboard((Display *)display, time);
}

//void XlibWrapper::XGrabServer(long display);
//void XlibWrapper::XUngrabServer(long display);

void XlibWrapper::XMapWindow(long display, long window) {
	LOGD("XlibWrapper::%s(%lX,%lX)", __FUNCTION__, display, window);
	//if (!window) throw NullPointerException();
	::XMapWindow((Display *)display, window);
}
void XlibWrapper::XUnmapWindow(long display, long window) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XUnmapWindow((Display *)display, window);
}
//void XlibWrapper::XMapRaised(long display, long window);
//void XlibWrapper::XRaiseWindow(long display, long window);

//void XlibWrapper::XLowerWindow(long display, long window);
//void XlibWrapper::XRestackWindows(long display, long windows, int length);
//void XlibWrapper::XConfigureWindow(long display, long window, long value_mask, long values);
//void XlibWrapper::XSetInputFocus(long display, long window);
//void XlibWrapper::XSetInputFocus2(long display, long window, long time);
//long XlibWrapper::XGetInputFocus(long display);

void XlibWrapper::XSelectInput(long display, long window, long event_mask) {
	LOGD("XlibWrapper::%s(%lX,%ld,mask=%lX)",__FUNCTION__,display, window, event_mask);
	::XSelectInput((Display*)display, window, event_mask);
}
void XlibWrapper::XNextEvent(long display, long event_ptr) {
	::XNextEvent((Display*)display, (XEvent*)event_ptr);
	//int s = sizeof(XEvent);
	//char buf[2*s+1];
	//for (int i=0; i < s; ++i) sprintf(buf+2*i, "%02X", ((char*)event_ptr)[i]);
	//LOGD("XlibWrapper::%s ev.type=%d, evd[%d]=%s",__FUNCTION__,((XEvent*)event_ptr)->type, s, buf);
}
void XlibWrapper::XPeekEvent(long display, long event_ptr) {
	::XPeekEvent((Display*)display, (XEvent*)event_ptr);
	LOGD("XlibWrapper::%s ev.type=%d",__FUNCTION__,((XEvent*)event_ptr)->type);
}
//void XlibWrapper::XMaskEvent(long display, long event_mask, long event_return);
//void XlibWrapper::XWindowEvent(long display, long window, long event_mask, long event_return);
boolean XlibWrapper::XFilterEvent(long event_ptr, long window) {
	return ::XFilterEvent((XEvent*)event_ptr, window);
}
boolean XlibWrapper::XSupportsLocale() {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return ::XSupportsLocale();
}
String XlibWrapper::XSetLocaleModifiers(const String& modifier_list) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return ::XSetLocaleModifiers(modifier_list.cstr());
}
//int XlibWrapper::XTranslateCoordinates( long display, long src_w, long dest_w, long src_x, long src_y, long dest_x_return, long dest_y_return, long child_return);
void XlibWrapper::XFlush(long display) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XFlush((Display*)display);
}
void XlibWrapper::XSync(long display,int discard) {
	::XSync((Display*)display,discard);
}
void XlibWrapper::XMoveResizeWindow(long display, long window, int x, int y, int width, int height) {
	LOGD("XlibWrapper::%s(%d,%d,%d,%d)",__FUNCTION__, x, y, width, height);
	::XMoveResizeWindow((Display*)display, window, x, y, width, height);
}
void XlibWrapper::XResizeWindow(long display, long window, int width, int height) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XResizeWindow((Display*)display, window, width, height);
}
void XlibWrapper::XMoveWindow(long display, long window, int x, int y) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XMoveWindow((Display*)display, window, x, y);
}

boolean XlibWrapper::XQueryPointer(long display, long window, long& root_return, long& child_return, long& root_x_return, long& root_y_return, long& win_x_return, long& win_y_return, long& mask_return) {
	Window l_root_return, l_child_return;
	int l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return;
	unsigned l_mask_return;
	LOGD("XlibWrapper::%s",__FUNCTION__);
	boolean r = ::XQueryPointer((Display*)display, window, &l_root_return, &l_child_return, &l_root_x_return, &l_root_y_return, &l_win_x_return, &l_win_y_return, &l_mask_return);
	root_return = (long)l_root_return;
	return r;
}
//void XlibWrapper::XFreeCursor(long display, long cursor);
void XlibWrapper::XSetWindowBackground(long display, long window, long background_pixel) {
	background_pixel &= 0xffffffff;
	LOGD("XlibWrapper::%s(%lX,%lX,bg_pixel=%lx)",__FUNCTION__,display,window,background_pixel);
	::XSetWindowBackground((Display*)display, window, background_pixel);
}
int XlibWrapper::XEventsQueued(long display, int mode) {
	return ::XEventsQueued((Display*)display, mode);
}

//int XlibWrapper::XInternAtoms(long display, const Array<String>& names, boolean only_if_exists, long atoms);
long XlibWrapper::InternAtom(long display, const String& string, int only_if_exists) {
	return ::XInternAtom((Display*)display, string.cstr(), only_if_exists);
}
void XlibWrapper::SetProperty(long display, long window, long atom, const String& str) {
	::XTextProperty tp;
	char *cname = const_cast<char*>(str.cstr());
	// int Xutf8TextListToTextProperty(Display *display, char **list, int count, XICCEncodingStyle style, XTextProperty *text_prop_return); 
	int status = ::Xutf8TextListToTextProperty((Display *)display, &cname, 1, ::XStdICCTextStyle, &tp);
	if (status == Success || status > 0) {
		::XChangeProperty((Display*)display, window, atom, tp.encoding, tp.format, PropModeReplace, tp.value, (int)tp.nitems);
		if (tp.value) ::XFree(tp.value);
	}
}
String XlibWrapper::GetProperty(long display, long window, long atom) {
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long bytes_after;
	unsigned char *string;
	int status = ::XGetWindowProperty((Display*)display, window, atom, 0, 0xFFFF, False, XA_STRING,
		&actual_type, &actual_format, &nitems, &bytes_after, &string);
	if (status != Success || string == NULL) {
		return "";
	}
	String res = (char*)string;
	XFree(string);
	return res;
}
int XlibWrapper::XGetWindowProperty(long display, long window, long atom, long offset, long length, boolean del, long req_type, long& actualy_type, int& actualy_format, long& nitems, long& bytes_after, long& data_ptr) {
	Atom actual_type;
	int actual_format;
	unsigned long _nitems;
	unsigned long _bytes_after;
	unsigned char *data;
	int status = ::XGetWindowProperty((Display*)display, window, atom, offset, length, del, req_type,
		&actual_type, &actual_format, &_nitems, &_bytes_after, &data);
	actualy_type = actual_type;
	actualy_format = actual_format;
	nitems = _nitems;
	bytes_after = _bytes_after;
	data_ptr = (long)data;
	return status;
}
void XlibWrapper::XChangePropertyImpl(long display, long window, long atom, long type, int format, int mode, void* data, int nelements) {
	::XChangeProperty((Display*)display, window, atom, type, format, mode, (unsigned char*)data, nelements);
}
void XlibWrapper::XChangeProperty(long display, long window, long atom, long type, int format, int mode, void* data, int nelements) {
	//TODO caching
	XChangePropertyImpl(display, window, atom, type, format, mode, data, nelements);
}

void XlibWrapper::XChangePropertyS(long display, long window, long atom, long type, int format, int mode, const String& value) {
	::XChangeProperty((Display*)display, window, atom, type, format, mode, (unsigned char*)value.cstr(), value.length());
}
void XlibWrapper::XDeleteProperty(long display, long window, long atom) {
	::XDeleteProperty((Display*)display, window, atom);
}

//void XlibWrapper::XSetTransientFor(long display, long window, long transient_for_window);
void XlibWrapper::XSetWMProtocols(long display, long window, long* wmprots, int count) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XSetWMProtocols((Display*)display, window, (Atom*)wmprots, count);
}
void XlibWrapper::XSetWMHints(long display, long window, long wmhints) {
}
//void XlibWrapper::XGetWMHints(long display, long window, long wmhints);
//long XlibWrapper::XAllocWMHints();
//int XlibWrapper::XGetPointerMapping(long display, long map, int buttonNumber);
//String XlibWrapper::XGetDefault(long display, const String& program, const String& option);
long XlibWrapper::getScreenOfWindow(long display, long window) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return (long)::_XScreenOfWindow((Display*)display, window);
}
long XlibWrapper::XScreenNumberOfScreen(long screen) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return ::XScreenNumberOfScreen((Screen*)screen);
}
//int XlibWrapper::XIconifyWindow(long display, long window, long screenNumber);
//String XlibWrapper::ServerVendor(long display);
//int XlibWrapper::VendorRelease(long display);
//boolean XlibWrapper::IsXsunKPBehavior(long display);
//boolean XlibWrapper::IsSunKeyboard(long display);
//boolean XlibWrapper::IsKanaKeyboard(long display);

//void XlibWrapper::XBell(long display, int percent);
int XlibWrapper::XCreateFontCursor(long display, int shape) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return (int)::XCreateFontCursor((Display*)display, shape);
}
//long XlibWrapper::XCreateBitmapFromData(long display, long drawable, long data, int width, int height);
//void XlibWrapper::XFreePixmap(long display, long pixmap);
//long XlibWrapper::XCreatePixmapCursor(long display, long source, long mask, long fore, long back, int x, int y);
//boolean XlibWrapper::XQueryBestCursor(long display, long drawable, int width, int height, long width_return, long height_return);
boolean XlibWrapper::XAllocColor(long display, long colormap, int& screen_in_out) {
	XColor c;
	int r = ((screen_in_out>>16)&0xff) * 256;
	int g = ((screen_in_out>>8)&0xff) * 256;
	int b = (screen_in_out&0xff) * 256;
	c.red = (short)r;
	c.green = (short)g;
	c.blue = (short)b;
	boolean ret =  ::XAllocColor((Display*)display, colormap, &c);
	screen_in_out = (int)c.pixel;
	return ret;
}

//long XlibWrapper::SetToolkitErrorHandler();
//void XlibWrapper::XSetErrorHandler(long handler);
//int XlibWrapper::CallErrorHandler(long handler, long display, long event_ptr);
//void XlibWrapper::XChangeWindowAttributes(long display, long window, long valuemask, long attributes);
//int XlibWrapper::XGetWindowAttributes(long display, long window, long attr_ptr);
//int XlibWrapper::XGetGeometry(long display, long drawable, long root_return, long x_return, long y_return, long width_return, long height_return, long border_width_return, long depth_return);

//int XlibWrapper::XGetWMNormalHints(long display, long window, long hints, long supplied_return);
void XlibWrapper::XSetWMNormalHints(long display, long window, long hints) {
	/*int s = 80;
	char buf[2*s+1];
	for (int i=0; i < s; ++i) sprintf(buf+2*i, "%02X", ((char*)hints)[i]);
	LOGD("XlibWrapper::%s(hints=%s)",__FUNCTION__, buf);*/
	XSizeHints *xh = (XSizeHints *)hints;
	LOGD("XlibWrapper::%s(f=%X, x=%d,y=%d,w=%d,h=%d)",__FUNCTION__, xh->flags, xh->x, xh->y, xh->width, xh->height);
	::XSetWMNormalHints((Display*)display, window, (XSizeHints*)hints);
}
void XlibWrapper::XSetMinMaxHints(long display, long window, int x, int y, int width, int height, long flags) {
	LOGD("XlibWrapper::%s(f=%X, x=%d,y=%d,w=%d,h=%d)",__FUNCTION__,flags, x,y,width,height);
    XSizeHints hints;
    hints.flags = flags;
    hints.width = width;
    hints.min_width = width;
    hints.max_width = width;
    hints.height = height;
    hints.min_height = height;
    hints.max_height = height;
    hints.x = x;
    hints.y = y;
    ::XSetWMNormalHints((Display*)display, window, &hints);
}
long XlibWrapper::XAllocSizeHints() {
	return (long)XAllocSizeHints();
}

//int XlibWrapper::XSendEvent(long display, long window, boolean propagate, long event_mask, long event);
//void XlibWrapper::XPutBackEvent(long display, long event);
//int XlibWrapper::XQueryTree(long display, long window, long root_return, long parent_return, long children_return, long nchildren_return);
long XlibWrapper::XGetVisualInfo(long display, long vinfo_mask, long vinfo_template, long& nitems_return) {
	int nitems;
	XVisualInfo *vi = ::XGetVisualInfo((Display*)display, vinfo_mask, (XVisualInfo*)vinfo_template, &nitems);
	nitems_return = nitems;
	return (long)vi;
}
//void XlibWrapper::XReparentWindow(long display, long window, long parent, int x, int y);

//void XlibWrapper::XConvertSelection(long display, long selection, long target, long property, long requestor, long time);

//void XlibWrapper::XSetSelectionOwner(long display, long selection, long owner, long time);

//long XlibWrapper::XGetSelectionOwner(long display, long selection);

String XlibWrapper::XGetAtomName(long display, long atom) {
	return ::XGetAtomName((Display*)display, atom);
}

//long XlibWrapper::XMaxRequestSize(long display);


//long XCreatePixmap(long display, long drawable, int width, int height, int depth);
//long XCreateImage(long display, long visual_ptr, int depth, int format, int offset, long data, int width, int height, int bitmap_pad, int bytes_per_line);
//void XDestroyImage(long image);
//void XPutImage(long display, long drawable, long gc, long image, int src_x, int src_y, int dest_x, int dest_y, int width, int height);
//long XCreateGC(long display, long drawable, long valuemask, long values);
//void XFreeGC(long display, long gc);
//void XSetWindowBackgroundPixmap(long display, long window, long pixmap);
void XlibWrapper::XClearWindow(long display, long window) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	::XCreateFontCursor((Display*)display, (unsigned)window);
}
//int XGetIconSizes(long display, long window, long ret_sizes, long ret_count);
//int XdbeQueryExtension(long display, long major_version_return, long minor_version_return);
//boolean XQueryExtension(long display, const String& name, long mop_return, long feve_return, long err_return);
//boolean IsKeypadKey(long keysym);
//long XdbeAllocateBackBufferName(long display, long window, int swap_action);
//int XdbeDeallocateBackBufferName(long display, long buffer);
//int XdbeBeginIdiom(long display);
//int XdbeEndIdiom(long display);
//int XdbeSwapBuffers(long display, long swap_info, int num_windows);

//void XQueryKeymap(long display, long vector);
//long XKeycodeToKeysym(long display, int keycode, int index);

//int XKeysymToKeycode(long display, long keysym);

    // xkb-related
//int XkbGetEffectiveGroup(long display);
//long XkbKeycodeToKeysym(long display, int keycode, int group, int level);
//void XkbSelectEvents(long display, long device, long bits_to_change, long values_for_bits);
//void XkbSelectEventDetails(long display, long device, long event_type, long bits_to_change, long values_for_bits);
//boolean XkbQueryExtension(long display, long opcode_rtrn, long event_rtrn, long error_rtrn, long major_in_out, long minor_in_out);
//boolean XkbLibraryVersion(long lib_major_in_out, long lib_minor_in_out);
//long XkbGetMap(long display, long which, long device_spec);
//long XkbGetUpdatedMap(long display, long which, long xkb);
//void XkbFreeKeyboard(long xkb, long which, boolean free_all);
//boolean XkbTranslateKeyCode(long xkb, int keycode, long mods, long mods_rtrn, long keysym_rtrn);
//void XkbSetDetectableAutoRepeat(long display, boolean detectable);

//void XConvertCase(long keysym, long keysym_lowercase, long keysym_uppercase); 
//long XGetModifierMapping(long display);
//void XFreeModifiermap(long keymap);
//void XRefreshKeyboardMapping(long event);

//void XChangeActivePointerGrab(long display, int mask, long cursor, long time);
//int XSynchronize(long display, boolean onoff);
boolean XlibWrapper::XNextSecondaryLoopEvent(long display, long event_ptr) {
	LOGD("XlibWrapper::%s",__FUNCTION__);
	return false;
}
//void ExitSecondaryLoop();
//Array<String> XTextPropertyToStringList(const Array<byte>& bytes, long encoding_atom);
//void SetBitmapShape(long display, long window, int width, int height, const Array<int>& bitmap);
//void SetZOrder(long display, long window, long above);
//void PrintXErrorEvent(long display, long event_ptr);

}}
