#include <lang/System.hpp>

#include "XlibWrapper.hpp"
#include <X11/Xlib.h>
#include <X11/Xlibint.h>

namespace awt { namespace x11 {

long XlibWrapper::XOpenDisplay(const String& name) {
	LOGD(__FUNCTION__);
	return (long)::XOpenDisplay(name.cstr());
}
//void XlibWrapper::XCloseDisplay(long display);
//long XlibWrapper::XDisplayString(long display);
//void XlibWrapper::XSetCloseDownMode(long display, int close_mode);

int XlibWrapper::XDefaultScreen(long display) {
	int r = ::XDefaultScreen((Display *)display);
	LOGD("%s(%lX) = %d", __FUNCTION__, display, r);
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
	LOGD("%s(%lX,%ld) = %d", __FUNCTION__, display, screen, r);
	return r;
}
//int XlibWrapper::ScreenCount(long display);

long XlibWrapper::XBlackPixel(long display, long screen) {
	LOGD(__FUNCTION__);
	return (long)::XBlackPixel((Display *)display, (int)screen);
}
long XlibWrapper::XWhitePixel(long display, long screen) {
	LOGD(__FUNCTION__);
	return (long)::XWhitePixel((Display *)display, (int)screen);
}

long XlibWrapper::XCreateWindow(long display, long parent, int x,int  y, int width, int height, int border_width, int depth, long wclass, long visual, long valuemask, void* attributes) {
	long r = ::XCreateWindow((Display *)display, parent, x, y, width, height, border_width, depth, (int)wclass, (Visual*)visual, valuemask, (XSetWindowAttributes*)attributes);
	LOGD("%s(%lX,%ld,bounds=(%d,%d,%d,%d),depth=%d) = %lX", __FUNCTION__, display, parent, x, y, width, height, depth, r);
	return r;
}
void XlibWrapper::XDestroyWindow(long display, long window) {
	::XDestroyWindow((Display *)display, window);
}

//int XlibWrapper::XGrabPointer(long display, long grab_window, int owner_events, int event_mask, int pointer_mode, int keyboard_mode, long confine_to, long cursor, long time);

void XlibWrapper::XUngrabPointer(long display, long time) {
	LOGD(__FUNCTION__);
	::XUngrabPointer((Display *)display, time);
}

//int XlibWrapper::XGrabKeyboard(long display, long grab_window, int owner_events, int pointer_mode, int keyboard_mode, long time);

void XlibWrapper::XUngrabKeyboard(long display, long time) {
	LOGD(__FUNCTION__);
	::XUngrabKeyboard((Display *)display, time);
}

//void XlibWrapper::XGrabServer(long display);
//void XlibWrapper::XUngrabServer(long display);

void XlibWrapper::XMapWindow(long display, long window) {
	LOGD("%s(%lX,%lX)", __FUNCTION__, display, window);
	//if (!window) throw NullPointerException();
	::XMapWindow((Display *)display, window);
}
void XlibWrapper::XUnmapWindow(long display, long window) {
	LOGD(__FUNCTION__);
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
	LOGD(__FUNCTION__);
	::XSelectInput((Display*)display, window, event_mask);
}
void XlibWrapper::XNextEvent(long display, void* ptr) {
	LOGD(__FUNCTION__);
	::XNextEvent((Display*)display, (XEvent*)ptr);
}
//void XlibWrapper::XMaskEvent(long display, long event_mask, long event_return);
//void XlibWrapper::XWindowEvent(long display, long window, long event_mask, long event_return);
//boolean XlibWrapper::XFilterEvent(void* ptr, long window);
boolean XlibWrapper::XSupportsLocale() {
	LOGD(__FUNCTION__);
	return ::XSupportsLocale();
}
String XlibWrapper::XSetLocaleModifiers(const String& modifier_list) {
	LOGD(__FUNCTION__);
	return ::XSetLocaleModifiers(modifier_list.cstr());
}
//int XlibWrapper::XTranslateCoordinates( long display, long src_w, long dest_w, long src_x, long src_y, long dest_x_return, long dest_y_return, long child_return);
//void XlibWrapper::XPeekEvent(long display,void* ptr);
void XlibWrapper::XFlush(long display) {
	LOGD(__FUNCTION__);
	::XFlush((Display*)display);
}
//void XlibWrapper::XSync(long display,int discard);
void XlibWrapper::XMoveResizeWindow(long display, long window, int x, int y, int width, int height) {
	LOGD(__FUNCTION__);
	::XMoveResizeWindow((Display*)display, window, x, y, width, height);
}
//void XlibWrapper::XResizeWindow(long display, long window, int width, int height);
//void XlibWrapper::XMoveWindow(long display, long window, int x, int y);

//int XQueryPointer(Display*, Window, Window*, Window*, int*, int*, int*, int*, unsigned int*)
boolean XlibWrapper::XQueryPointer(long display, long window, long& root_return, long& child_return, long& root_x_return, long& root_y_return, long& win_x_return, long& win_y_return, long& mask_return) {
	Window l_root_return, l_child_return;
	int l_root_x_return, l_root_y_return, l_win_x_return, l_win_y_return;
	unsigned l_mask_return;
	LOGD(__FUNCTION__);
	boolean r = ::XQueryPointer((Display*)display, window, &l_root_return, &l_child_return, &l_root_x_return, &l_root_y_return, &l_win_x_return, &l_win_y_return, &l_mask_return);
	root_return = (long)l_root_return;
	return r;
}
//void XlibWrapper::XFreeCursor(long display, long cursor);
//void XlibWrapper::XSetWindowBackground(long display, long window, long background_pixel);
int XlibWrapper::XEventsQueued(long display, int mode) {
	return ::XEventsQueued((Display*)display, mode);
}
//int XlibWrapper::XInternAtoms(long display, const Array<String>& names, boolean only_if_exists, long atoms);

//void XlibWrapper::SetProperty(long display, long window, long atom, const String& str);
//String XlibWrapper::GetProperty(long display ,long window, long atom);
//long XlibWrapper::InternAtom(long display, const String& string, int only_if_exists);
//int XlibWrapper::XGetWindowProperty(long display, long window, long atom, long long_offset, long long_length, long del, long req_type, long actualy_type, long actualy_format, long nitems_ptr, long bytes_after, long data_ptr);
//void XlibWrapper::XChangePropertyImpl(long display, long window, long atom, long type, int format, int mode, long data, int nelements);
//void XlibWrapper::XChangeProperty(long display, long window, long atom, long type, int format, int mode, long data, int nelements);

//void XlibWrapper::XChangePropertyS(long display, long window, long atom, long type, int format, int mode, const String& value);
//void XlibWrapper::XDeleteProperty(long display, long window, long atom);

//void XlibWrapper::XSetTransientFor(long display, long window, long transient_for_window);
//void XlibWrapper::XSetWMHints(long display, long window, long wmhints);
//void XlibWrapper::XGetWMHints(long display, long window, long wmhints);
//long XlibWrapper::XAllocWMHints();
//int XlibWrapper::XGetPointerMapping(long display, long map, int buttonNumber);
//String XlibWrapper::XGetDefault(long display, const String& program, const String& option);
long XlibWrapper::getScreenOfWindow(long display, long window) {
	LOGD(__FUNCTION__);
	return (long)::_XScreenOfWindow((Display*)display, (int)window);
}
long XlibWrapper::XScreenNumberOfScreen(long screen) {
	LOGD(__FUNCTION__);
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
	LOGD(__FUNCTION__);
	return (int)::XCreateFontCursor((Display*)display, shape);
}
//long XlibWrapper::XCreateBitmapFromData(long display, long drawable, long data, int width, int height);
//void XlibWrapper::XFreePixmap(long display, long pixmap);
//long XlibWrapper::XCreatePixmapCursor(long display, long source, long mask, long fore, long back, int x, int y);
//boolean XlibWrapper::XQueryBestCursor(long display, long drawable, int width, int height, long width_return, long height_return);
//boolean XlibWrapper::XAllocColor( long display, long colormap, long screen_in_out);


//long XlibWrapper::SetToolkitErrorHandler();
//void XlibWrapper::XSetErrorHandler(long handler);
//int XlibWrapper::CallErrorHandler(long handler, long display, long event_ptr);
//void XlibWrapper::XChangeWindowAttributes(long display, long window, long valuemask, long attributes);
//int XlibWrapper::XGetWindowAttributes(long display, long window, long attr_ptr);
//int XlibWrapper::XGetGeometry(long display, long drawable, long root_return, long x_return, long y_return, long width_return, long height_return, long border_width_return, long depth_return);

//int XlibWrapper::XGetWMNormalHints(long display, long window, long hints, long supplied_return);
//void XlibWrapper::XSetWMNormalHints(long display, long window, long hints);
//void XlibWrapper::XSetMinMaxHints(long display, long window, int x, int y, int width, int height, long flags);
//long XlibWrapper::XAllocSizeHints();

//int XlibWrapper::XSendEvent(long display, long window, boolean propagate, long event_mask, long event);
//void XlibWrapper::XPutBackEvent(long display, long event);
//int XlibWrapper::XQueryTree(long display, long window, long root_return, long parent_return, long children_return, long nchildren_return);
//long XlibWrapper::XGetVisualInfo(long display, long vinfo_mask, long vinfo_template, long nitems_return);
//void XlibWrapper::XReparentWindow(long display, long window, long parent, int x, int y);

//void XlibWrapper::XConvertSelection(long display, long selection, long target, long property, long requestor, long time);

//void XlibWrapper::XSetSelectionOwner(long display, long selection, long owner, long time);

//long XlibWrapper::XGetSelectionOwner(long display, long selection);

//String XlibWrapper::XGetAtomName(long display, long atom);

//long XlibWrapper::XMaxRequestSize(long display);


//long XCreatePixmap(long display, long drawable, int width, int height, int depth);
//long XCreateImage(long display, long visual_ptr, int depth, int format, int offset, long data, int width, int height, int bitmap_pad, int bytes_per_line);
//void XDestroyImage(long image);
//void XPutImage(long display, long drawable, long gc, long image, int src_x, int src_y, int dest_x, int dest_y, int width, int height);
//long XCreateGC(long display, long drawable, long valuemask, long values);
//void XFreeGC(long display, long gc);
//void XSetWindowBackgroundPixmap(long display, long window, long pixmap);
//void XClearWindow(long display, long window);
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
boolean XlibWrapper::XNextSecondaryLoopEvent(long display, void *ptr) {
	LOGD(__FUNCTION__);
	return false;
}
//void ExitSecondaryLoop();
//Array<String> XTextPropertyToStringList(const Array<byte>& bytes, long encoding_atom);
//void SetBitmapShape(long display, long window, int width, int height, const Array<int>& bitmap);
//void SetZOrder(long display, long window, long above);
//void PrintXErrorEvent(long display, long event_ptr);

}}
