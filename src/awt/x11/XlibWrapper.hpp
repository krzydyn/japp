#ifndef __AWT_X11_LIBWRAPPER_HPP
#define __AWT_X11_LIBWRAPPER_HPP

#include <lang/String.hpp>

namespace awt { namespace x11 {

enum class XCursorFontConstants : int {
	XC_X_cursor=0,
	XC_arrow=2,
};

// http://www.grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/awt/X11/XlibWrapper.java
class XlibWrapper {
private:
	XlibWrapper() {}
	static int XDefaultScreen(long display);
	static long XRootWindow(long display, long screen);
	static long XBlackPixel(long display, long screen);
	static long XWhitePixel(long display, long screen);
public:
	static long XOpenDisplay(const String& name);
	static void XCloseDisplay(long display);
	static int XScreenCount(long display);
	static long XDisplayString(long display);
	static void XSetCloseDownMode(long display, int close_mode);
	static int DefaultScreen(long display) {return XDefaultScreen(display);}
	static long ScreenOfDisplay(long display, long screen_number);
	static int DoesBackingStore(long screen);
	static long DisplayWidth(long display, long screen);
	static long DisplayWidthMM(long display, long screen);
	static long DisplayHeight(long display, long screen);
	static long DisplayHeightMM(long display, long screen);
	static long RootWindow(long display, long screen) { return XRootWindow(display, screen); }
	static int ScreenCount(long display);

	static long BlackPixel(long display, int screen) { return XBlackPixel(display, screen); }
	static long WhitePixel(long display, int screen) { return XWhitePixel(display, screen); }

	static long XCreateWindow(long display, long parent, int x,int  y, int width, int height, int border_width, int depth, long wclass, long visual, long valuemask, void* attributes);
	static void XDestroyWindow(long display, long window);

	static int XGrabPointer(long display, long grab_window,
                                   int owner_events, int event_mask, int pointer_mode,
                                   int keyboard_mode, long confine_to, long cursor, long time);

	static void XUngrabPointer(long display, long time);

	static int XGrabKeyboard(long display, long grab_window,
                                    int owner_events, int pointer_mode,
                                    int keyboard_mode, long time);

	static void XUngrabKeyboard(long display, long time);

	static void XGrabServer(long display);
	static void XUngrabServer(long display);

	static void XMapWindow(long display, long window);
	static void XUnmapWindow(long display, long window);
	static void XMapRaised(long display, long window);
	static void XRaiseWindow(long display, long window);

	static void XLowerWindow(long display, long window);
	static void XRestackWindows(long display, long windows, int length);
	static void XConfigureWindow(long display, long window, long value_mask, long values);
	static void XSetInputFocus(long display, long window);
	static void XSetInputFocus2(long display, long window, long time);
	static long XGetInputFocus(long display);

	static void XSelectInput(long display, long window, long event_mask);
	static void XNextEvent(long display,void* ptr);
	static void XMaskEvent(long display, long event_mask, long event_return);
	static void XWindowEvent(long display, long window, long event_mask, long event_return);
	static boolean XFilterEvent(void* ptr, long window);
	static boolean XSupportsLocale();
	static String XSetLocaleModifiers(const String& modifier_list);
	static int XTranslateCoordinates(
                                             long display, long src_w, long dest_w,
                                             long src_x, long src_y,
                                             long dest_x_return, long dest_y_return,
                                             long child_return);
	static void XPeekEvent(long display,void* ptr);
	static void XFlush(long display);
	static void XSync(long display,int discard);
	static void XMoveResizeWindow(long display, long window, int x, int y, int width, int height);
	static void XResizeWindow(long display, long window, int width, int height);
	static void XMoveWindow(long display, long window, int x, int y);

	static boolean XQueryPointer(long display, long window, long& root_return, long& child_return, long& root_x_return, long& root_y_return, long& win_x_return, long& win_y_return, long& mask_return);
	static void XFreeCursor(long display, long cursor);
	static void XSetWindowBackground(long display, long window, long background_pixel);
	static int XEventsQueued(long display, int mode);
	static int XInternAtoms(long display, const Array<String>& names, boolean only_if_exists, long atoms);

	static void SetProperty(long display, long window, long atom, const String& str);
	static String GetProperty(long display ,long window, long atom);
	static long InternAtom(long display, const String& string, int only_if_exists);
	static int XGetWindowProperty(long display, long window, long atom,
                                         long long_offset, long long_length,
                                         long del, long req_type, long actualy_type,
                                         long actualy_format, long nitems_ptr,
                                         long bytes_after, long data_ptr);
   static void XChangePropertyImpl(long display, long window, long atom,
                                           long type, int format, int mode, long data,
                                           int nelements);
	static void XChangeProperty(long display, long window, long atom,
                                long type, int format, int mode, long data,
                                int nelements);

	static void XChangePropertyS(long display, long window, long atom,
                                       long type, int format, int mode, const String& value);
	static void XDeleteProperty(long display, long window, long atom);

	static void XSetTransientFor(long display, long window, long transient_for_window);
	static void XSetWMHints(long display, long window, long wmhints);
	static void XGetWMHints(long display, long window, long wmhints);
	static long XAllocWMHints();
	static int XGetPointerMapping(long display, long map, int buttonNumber);
	static String XGetDefault(long display, const String& program, const String& option);
	static long getScreenOfWindow(long display, long window);
	static long XScreenNumberOfScreen(long screen);
	static int XIconifyWindow(long display, long window, long screenNumber);
	static String ServerVendor(long display);
	static int VendorRelease(long display);
	static boolean IsXsunKPBehavior(long display);
	static boolean IsSunKeyboard(long display);
	static boolean IsKanaKeyboard(long display);

	static void XBell(long display, int percent);
	static int XCreateFontCursor(long display, int shape);
	static long XCreateBitmapFromData(long display, long drawable, long data, int width, int height);
	static void XFreePixmap(long display, long pixmap);
	static long XCreatePixmapCursor(long display, long source, long mask, long fore, long back, int x, int y);
	static boolean XQueryBestCursor(long display, long drawable, int width, int height, long width_return, long height_return);
	static boolean XAllocColor( long display, long colormap, int& screen_in_out);


	static long SetToolkitErrorHandler();
	static void XSetErrorHandler(long handler);
	static int CallErrorHandler(long handler, long display, long event_ptr);
	static void XChangeWindowAttributes(long display, long window, long valuemask, long attributes);
	static int XGetWindowAttributes(long display, long window, long attr_ptr);
	static int XGetGeometry(long display, long drawable, long root_return, long x_return, long y_return,
                                   long width_return, long height_return, long border_width_return, long depth_return);

	static int XGetWMNormalHints(long display, long window, long hints, long supplied_return);
	static void XSetWMNormalHints(long display, long window, long hints);
	static void XSetMinMaxHints(long display, long window, int x, int y, int width, int height, long flags);
	static long XAllocSizeHints();

	static int XSendEvent(long display, long window, boolean propagate, long event_mask, long event);
	static void XPutBackEvent(long display, long event);
	static int XQueryTree(long display, long window, long root_return, long parent_return, long children_return, long nchildren_return);
	static long XGetVisualInfo(long display, long vinfo_mask, long vinfo_template, long nitems_return);
	static void XReparentWindow(long display, long window, long parent, int x, int y);

	static void XConvertSelection(long display, long selection,
                                         long target, long property,
                                         long requestor, long time);

	static void XSetSelectionOwner(long display, long selection,
                                          long owner, long time);

	static long XGetSelectionOwner(long display, long selection);

	static String XGetAtomName(long display, long atom);

	static long XMaxRequestSize(long display);


	static long XCreatePixmap(long display, long drawable, int width, int height, int depth);
	static long XCreateImage(long display, long visual_ptr, int depth, int format,
                                    int offset, long data, int width, int height, int bitmap_pad,
                                    int bytes_per_line);
	static void XDestroyImage(long image);
	static void XPutImage(long display, long drawable, long gc, long image,
                                 int src_x, int src_y, int dest_x, int dest_y,
                                 int width, int height);
	static long XCreateGC(long display, long drawable, long valuemask, long values);
	static void XFreeGC(long display, long gc);
	static void XSetWindowBackgroundPixmap(long display, long window, long pixmap);
	static void XClearWindow(long display, long window);
	static int XGetIconSizes(long display, long window, long ret_sizes, long ret_count);
	static int XdbeQueryExtension(long display, long major_version_return,
                                         long minor_version_return);
	static boolean XQueryExtension(long display, const String& name, long mop_return,
                                         long feve_return, long err_return);
	static boolean IsKeypadKey(long keysym);
	static long XdbeAllocateBackBufferName(long display, long window, int swap_action);
	static int XdbeDeallocateBackBufferName(long display, long buffer);
	static int XdbeBeginIdiom(long display);
	static int XdbeEndIdiom(long display);
	static int XdbeSwapBuffers(long display, long swap_info, int num_windows);

	static void XQueryKeymap(long display, long vector);
	static long XKeycodeToKeysym(long display, int keycode, int index);

	static int XKeysymToKeycode(long display, long keysym);

    // xkb-related
	static int XkbGetEffectiveGroup(long display);
	static long XkbKeycodeToKeysym(long display, int keycode, int group, int level);
	static void XkbSelectEvents(long display, long device, long bits_to_change, long values_for_bits);
	static void XkbSelectEventDetails(long display, long device, long event_type,
                                              long bits_to_change, long values_for_bits);
	static boolean XkbQueryExtension(long display, long opcode_rtrn, long event_rtrn,
              long error_rtrn, long major_in_out, long minor_in_out);
	static boolean XkbLibraryVersion(long lib_major_in_out, long lib_minor_in_out);
	static long XkbGetMap(long display, long which, long device_spec);
	static long XkbGetUpdatedMap(long display, long which, long xkb);
	static void XkbFreeKeyboard(long xkb, long which, boolean free_all);
	static boolean XkbTranslateKeyCode(long xkb, int keycode, long mods, long mods_rtrn, long keysym_rtrn);
	static void XkbSetDetectableAutoRepeat(long display, boolean detectable);

	static void XConvertCase(long keysym, long keysym_lowercase, long keysym_uppercase); 
	static long XGetModifierMapping(long display);
	static void XFreeModifiermap(long keymap);
	static void XRefreshKeyboardMapping(long event);

	static void XChangeActivePointerGrab(long display, int mask, long cursor, long time);
	static int XSynchronize(long display, boolean onoff);
	static boolean XNextSecondaryLoopEvent(long display, void *ptr);
	static void ExitSecondaryLoop();
	static Array<String> XTextPropertyToStringList(const Array<byte>& bytes, long encoding_atom);
	static void SetBitmapShape(long display, long window, int width, int height, const Array<int>& bitmap);
	static void SetZOrder(long display, long window, long above);
	static void PrintXErrorEvent(long display, long event_ptr);
};

}}

#endif
